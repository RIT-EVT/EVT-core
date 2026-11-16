#include <core/io/platform/f3xx/DACf3xx.hpp>

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal.h>
#include <core/platform/f3xx/stm32f3xx.hpp>
#include <core/io/platform/f3xx/GPIOf3xx.hpp>
#include <core/utils/log.hpp>

namespace core::io {
namespace {

/** Hardware resources supporting a single DAC output pin */
struct Slot {
    DAC_TypeDef* dac;
    uint32_t channel;
    DMA_Channel_TypeDef* dma;
    IRQn_Type dmaIrq;
    uint32_t dmaReqMap; // Mask that needs to be set for this channels DMA request to reach DMA1 (F3 has no DMA2)
    uint32_t trigger;
    uint8_t timer;      // Index into timers[]
};

/** A basic timer that paces buffered output, shared by the channels that use it */
struct TriggerTimer {
    TIM_TypeDef* tim;
    uint32_t rate;
    uint8_t users; // Number of DAC instances sharing this timer
};

// Order must match slotFor() output
const Slot SLOTS[] = {
    {DAC1,
     DAC_CHANNEL_1,
     DMA1_Channel3,
     DMA1_Channel3_IRQn,
     HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3,
     DAC_TRIGGER_T6_TRGO,
     0},
#ifdef STM32F334x8
    {DAC1,
     DAC_CHANNEL_2,
     DMA1_Channel4,
     DMA1_Channel4_IRQn,
     HAL_REMAPDMA_TIM7_DAC1_CH2_DMA1_CH4,
     DAC_TRIGGER_T6_TRGO,
     0},
    {DAC2,
     DAC_CHANNEL_1,
     DMA1_Channel5,
     DMA1_Channel5_IRQn,
     HAL_REMAPDMA_DAC2_CH1_DMA1_CH5,
     DAC_TRIGGER_T7_TRGO,
     1},
#endif
};

constexpr uint8_t NUM_SLOTS = sizeof(SLOTS) / sizeof(SLOTS[0]);

TriggerTimer timers[] = {
    {TIM6, 0, 0},
#ifdef STM32F334x8
    {TIM7, 0, 0},
#endif
};

DAC_HandleTypeDef dacHandles[NUM_SLOTS];
DMA_HandleTypeDef dmaHandles[NUM_SLOTS];
DACf3xx* owners[NUM_SLOTS];

/**
 * Start a trigger timer at the given rate, or join it if it already runs at that rate.
 */
DACBase::DACStatus acquireTimer(uint8_t index, uint32_t rate) {
    TriggerTimer& timer = timers[index];

    if (timer.users > 0) {
        if (timer.rate != rate) {
            return DACBase::DACStatus::BUSY;
        }

        timer.users++;
        return DACBase::DACStatus::OK;
    }

    // Dynamically get clock freq; APB1 timers run at twice PCLK1 whenever the APB1 prescaler is not 1
    uint32_t clock = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) {
        clock *= 2;
    }

    /*
     * Split clock / rate into a 16-bit prescaler (PSC) and a 16-bit period (ARR)
     * sample_rate = clock / ((PSC + 1) x (ARR + 1))
     */
    uint32_t ticks     = clock / rate;
    uint32_t prescaler = (ticks - 1) / 65536;
    if (ticks < 2 || prescaler > 0xFFFF) {
        return DACBase::DACStatus::ERROR;
    }

    if (timer.tim == TIM6) {
        __HAL_RCC_TIM6_CLK_ENABLE();
    }
#ifdef STM32F334x8
    else {
        __HAL_RCC_TIM7_CLK_ENABLE();
    }
#endif

    TIM_TypeDef* tim = timer.tim;
    tim->CR1         = 0;               // Stop timer for configuration
    tim->PSC         = prescaler;
    tim->ARR         = ticks / (prescaler + 1) - 1;
    tim->EGR         = TIM_EGR_UG;      // Force update event to load PSC right away
    tim->SR          = 0;               // Clear update flag
    tim->CR2         = TIM_CR2_MMS_1;   // TRGO on every update event
    tim->CR1         = TIM_CR1_CEN;     // Start the timer

    timer.rate  = rate;
    timer.users = 1;
    return DACBase::DACStatus::OK;
}

/**
 * Drop one user from a trigger timer, stopping it when none remain.
 */
void releaseTimer(uint8_t index) {
    TriggerTimer& timer = timers[index];
    if (timer.users > 0 && --timer.users == 0) {
        timer.tim->CR1 = 0;
    }
}

/**
 * Pass a DMA event from the HAL to the channel that owns the DAC handle.
 */
void dispatch(DAC_HandleTypeDef* hdac, DACBase::DACEvent event) {
    DACf3xx* dac = owners[hdac - dacHandles];
    if (dac != nullptr) {
        dac->handleEvent(event);
    }
}

} // namespace

void DACf3xx::handleEvent(DACEvent event) {
    // On an error, or once DMA finishes a non-circular buffer, mark the buffer as no longer running.
    if (event == DACEvent::ERROR || (event == DACEvent::COMPLETE && !circular)) {
        bufferRunning = false;
    }
    notify(event);
}

DACf3xx::DACf3xx(Pin pin) : slot(slotFor(pin)) {
    const Slot& s           = SLOTS[slot];
    DAC_HandleTypeDef& hdac = dacHandles[slot];
    owners[slot]            = this;

    GPIO_InitTypeDef gpioInit;
    GPIOf3xx::gpioStateInit(&gpioInit, &pin, 1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);

    if (s.dac == DAC1) {
        __HAL_RCC_DAC1_CLK_ENABLE();
    }
#ifdef STM32F334x8
    else {
        __HAL_RCC_DAC2_CLK_ENABLE();
    }
#endif
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_DMA_REMAP_CHANNEL_ENABLE(s.dmaReqMap);

    hdac.Instance = s.dac;
    if (HAL_DAC_Init(&hdac) != HAL_OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "DAC init failed on pin 0x%x", static_cast<int>(pin));
        return;
    }
    dmaHandles[slot].Instance = s.dma;

    HAL_NVIC_SetPriority(s.dmaIrq, core::platform::DAC_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(s.dmaIrq);

    configureChannel(DAC_TRIGGER_NONE);
    HAL_DAC_Start(&hdac, s.channel);
    HAL_DAC_SetValue(&hdac, s.channel, DAC_ALIGN_12B_R, 0);
}

DACBase::DACStatus DACf3xx::startBuffer(const uint16_t* samples, uint16_t length, uint32_t sampleHz, bool circular) {
    if (samples == nullptr || length == 0 || sampleHz == 0 || sampleHz > MAX_SAMPLE_RATE) {
        return DACStatus::ERROR;
    }

    const Slot& s           = SLOTS[slot];
    DAC_HandleTypeDef& hdac = dacHandles[slot];
    DMA_HandleTypeDef& hdma = dmaHandles[slot];

    // Revert to static mode first, freeing this channel's hold on the timer to be restarted at new rate
    stopBuffer();

    DACStatus status = acquireTimer(s.timer, sampleHz);
    if (status != DACStatus::OK) {
        return status;
    }
    triggered = true;

    // The trigger can only be changed while the channel is disabled
    HAL_DAC_Stop(&hdac, s.channel);
    configureChannel(s.trigger);

    hdma.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma.Init.MemInc              = DMA_MINC_ENABLE;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hdma.Init.Mode                = circular ? DMA_CIRCULAR : DMA_NORMAL;
    hdma.Init.Priority            = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma) != HAL_OK) {
        stopBuffer();
        return DACStatus::ERROR;
    }
    if (s.channel == DAC_CHANNEL_1) {
        __HAL_LINKDMA(&hdac, DMA_Handle1, hdma);
    } else {
        __HAL_LINKDMA(&hdac, DMA_Handle2, hdma);
    }

    // Set before starting, since the first DMA interrupt can arrive immediately
    this->circular = circular;
    bufferRunning  = true;

    // The HAL takes uint32_t*, and technically wants no const, but HAL never writes and only reads as uint16_t
    uint32_t* data = (uint32_t*)samples;
    if (HAL_DAC_Start_DMA(&hdac, s.channel, data, length, DAC_ALIGN_12B_R) != HAL_OK) {
        stopBuffer();
        return DACStatus::ERROR;
    }

    __HAL_DAC_DISABLE_IT(&hdac, DAC_IT_DMAUDR1 << s.channel); // HAL auto-enables this but we don't want it

    return DACStatus::OK;
}

void DACf3xx::stopBuffer() {
    if (!triggered) {
        return;
    }

    const Slot& s           = SLOTS[slot];
    DAC_HandleTypeDef& hdac = dacHandles[slot];
    uint16_t last           = readRaw();

    // Also disables the channel. Returns an error after a finished one-shot buffer, which is harmless.
    HAL_DAC_Stop_DMA(&hdac, s.channel);
    bufferRunning = false;
    triggered     = false;
    releaseTimer(s.timer);

    configureChannel(DAC_TRIGGER_NONE);
    HAL_DAC_Start(&hdac, s.channel);
    HAL_DAC_SetValue(&hdac, s.channel, DAC_ALIGN_12B_R, last);
}

void DACf3xx::writeRaw(uint16_t value) {
    stopBuffer();
    HAL_DAC_SetValue(&dacHandles[slot], SLOTS[slot].channel, DAC_ALIGN_12B_R, value);
}

uint16_t DACf3xx::readRaw() const {
    return static_cast<uint16_t>(HAL_DAC_GetValue(&dacHandles[slot], SLOTS[slot].channel));
}

void DACf3xx::configureChannel(uint32_t trigger) {
    DAC_ChannelConfTypeDef config = {};
    config.DAC_Trigger            = trigger;
    config.DAC_OutputBuffer       = DAC_OUTPUTBUFFER_ENABLE;
#ifdef STM32F334x8
    // DAC1 channel 2 and DAC2 have an output switch instead of a buffer, which must be closed to drive the pin
    config.DAC_OutputSwitch = DAC_OUTPUTSWITCH_ENABLE;
#endif
    HAL_DAC_ConfigChannel(&dacHandles[slot], &config, SLOTS[slot].channel);
}

/*
 * HAL callbacks, called from HAL_DMA_IRQHandler() through the DAC's DMA handle
 */
extern "C" void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::HALF_COMPLETE);
}

extern "C" void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::COMPLETE);
}

extern "C" void HAL_DAC_ErrorCallbackCh1(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::ERROR);
}

#ifdef STM32F334x8
extern "C" void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::HALF_COMPLETE);
}

extern "C" void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::COMPLETE);
}

extern "C" void HAL_DACEx_ErrorCallbackCh2(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::ERROR);
}
#endif

/*
 * DMA interrupt vectors. Each is only enabled once its DAC channel has been created.
 */
extern "C" void DMA1_Channel3_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dmaHandles[0]);
}

#ifdef STM32F334x8
extern "C" void DMA1_Channel4_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dmaHandles[1]);
}

extern "C" void DMA1_Channel5_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dmaHandles[2]);
}
#endif

} // namespace core::io
