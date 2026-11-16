#include <core/io/platform/f4xx/DACf4xx.hpp>

#include <HALf4/stm32f4xx.h>
#include <core/platform/f4xx/stm32f4xx.hpp>
#include <core/io/platform/f4xx/GPIOf4xx.hpp>
#include <core/utils/log.hpp>

namespace core::io {
namespace {

/** Hardware resources supporting a single DAC output pin */
struct Slot {
    uint32_t channel;
    DMA_Stream_TypeDef* dma;
    IRQn_Type dmaIrq;
};

// Order must match slotFor() output. Both channels use DMA request channel 7.
const Slot SLOTS[] = {
    {DAC_CHANNEL_1, DMA1_Stream5, DMA1_Stream5_IRQn},
    {DAC_CHANNEL_2, DMA1_Stream6, DMA1_Stream6_IRQn},
};

constexpr uint8_t NUM_SLOTS = sizeof(SLOTS) / sizeof(SLOTS[0]);

DAC_HandleTypeDef dacHandles[NUM_SLOTS];
DMA_HandleTypeDef dmaHandles[NUM_SLOTS];
DACf4xx* owners[NUM_SLOTS];

/// Rate TIM7 is running at
uint32_t timerRate;
/// Number of DAC instances sharing TIM7
uint8_t timerUsers;

/**
 * Start TIM7 at the given rate, or join it if it already runs at that rate.
 */
DACBase::DACStatus acquireTimer(uint32_t rate) {
    if (timerUsers > 0) {
        if (timerRate != rate) {
            return DACBase::DACStatus::BUSY;
        }

        timerUsers++;
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

    __HAL_RCC_TIM7_CLK_ENABLE();
    TIM7->CR1 = 0;             // Stop timer for configuration
    TIM7->PSC = prescaler;
    TIM7->ARR = ticks / (prescaler + 1) - 1;
    TIM7->EGR = TIM_EGR_UG;    // Force update event to load PSC right away
    TIM7->SR  = 0;             // Clear update flag
    TIM7->CR2 = TIM_CR2_MMS_1; // TRGO on every update event
    TIM7->CR1 = TIM_CR1_CEN;   // Start the timer

    timerRate  = rate;
    timerUsers = 1;
    return DACBase::DACStatus::OK;
}

/**
 * Drop one user from TIM7, stopping it when none remain.
 */
void releaseTimer() {
    if (timerUsers > 0 && --timerUsers == 0) {
        TIM7->CR1 = 0;
    }
}

/**
 * Pass a DMA event from the HAL to the channel that owns the DAC handle.
 */
void dispatch(DAC_HandleTypeDef* hdac, DACBase::DACEvent event) {
    DACf4xx* dac = owners[hdac - dacHandles];
    if (dac != nullptr) {
        dac->handleEvent(event);
    }
}

} // namespace

void DACf4xx::handleEvent(DACEvent event) {
    // On an error, or once DMA finishes a non-circular buffer, mark the buffer as no longer running.
    if (event == DACEvent::ERROR || (event == DACEvent::COMPLETE && !circular)) {
        bufferRunning = false;
    }
    notify(event);
}

DACf4xx::DACf4xx(Pin pin) : slot(slotFor(pin)) {
    const Slot& s           = SLOTS[slot];
    DAC_HandleTypeDef& hdac = dacHandles[slot];
    owners[slot]            = this;

    GPIOf4xx::gpioSingleInit(pin, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);

    __HAL_RCC_DAC_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    hdac.Instance = DAC1;
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

DACBase::DACStatus DACf4xx::startBuffer(const uint16_t* samples, uint16_t length, uint32_t sampleHz, bool circular) {
    if (samples == nullptr || length == 0 || sampleHz == 0 || sampleHz > MAX_SAMPLE_RATE) {
        return DACStatus::ERROR;
    }

    const Slot& s           = SLOTS[slot];
    DAC_HandleTypeDef& hdac = dacHandles[slot];
    DMA_HandleTypeDef& hdma = dmaHandles[slot];

    // Revert to static mode first, freeing this channel's hold on the timer to be restarted at new rate
    stopBuffer();

    DACStatus status = acquireTimer(sampleHz);
    if (status != DACStatus::OK) {
        return status;
    }
    triggered = true;

    // The trigger can only be changed while the channel is disabled
    HAL_DAC_Stop(&hdac, s.channel);
    configureChannel(DAC_TRIGGER_T7_TRGO);

    hdma.Init.Channel             = DMA_CHANNEL_7;
    hdma.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma.Init.MemInc              = DMA_MINC_ENABLE;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hdma.Init.Mode                = circular ? DMA_CIRCULAR : DMA_NORMAL;
    hdma.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
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

void DACf4xx::stopBuffer() {
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
    releaseTimer();

    configureChannel(DAC_TRIGGER_NONE);
    HAL_DAC_Start(&hdac, s.channel);
    HAL_DAC_SetValue(&hdac, s.channel, DAC_ALIGN_12B_R, last);
}

void DACf4xx::writeRaw(uint16_t value) {
    stopBuffer();
    HAL_DAC_SetValue(&dacHandles[slot], SLOTS[slot].channel, DAC_ALIGN_12B_R, value);
}

uint16_t DACf4xx::readRaw() const {
    return static_cast<uint16_t>(HAL_DAC_GetValue(&dacHandles[slot], SLOTS[slot].channel));
}

void DACf4xx::configureChannel(uint32_t trigger) {
    DAC_ChannelConfTypeDef config = {};
    config.DAC_Trigger            = trigger;
    config.DAC_OutputBuffer       = DAC_OUTPUTBUFFER_ENABLE;
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

extern "C" void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::HALF_COMPLETE);
}

extern "C" void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::COMPLETE);
}

extern "C" void HAL_DACEx_ErrorCallbackCh2(DAC_HandleTypeDef* hdac) {
    dispatch(hdac, DACBase::DACEvent::ERROR);
}

/*
 * DMA interrupt vectors. Each is only enabled once its DAC channel has been created.
 */
extern "C" void DMA1_Stream5_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dmaHandles[0]);
}

extern "C" void DMA1_Stream6_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dmaHandles[1]);
}

} // namespace core::io
