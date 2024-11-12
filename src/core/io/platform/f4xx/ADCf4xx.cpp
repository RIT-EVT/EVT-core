/*
 * This DMA ADC is different than f3xx DMA ADC!
 * f4xx DMA ADC uses a timer to trigger ADC conversions. Timer frequency is 1kHz.
 *
 * Timers were added to slow down ADC DMA interrupts, as when they ADC DMA is allowed to convert constantly,
 * the interrupts stopped the program from doing anything else besides interrupt calls.
 *
 * WARNING: DOES NOT WORK ON PINS THAT ARE ALREADY IN USE (LIKE UART PINS)
 */

#include <HALf4/stm32f4xx.h>
#include <HALf4/stm32f4xx_hal_adc.h>
#include <core/io/pin.hpp>
#include <core/io/platform/f4xx/ADCf4xx.hpp>
#include <core/io/platform/f4xx/GPIOf4xx.hpp>
#include <core/platform/f4xx/stm32f4xx.hpp>
#include <core/utils/log.hpp>

namespace {
/// This is made as a static variable so that it is accessible in the interrupt.
DMA_HandleTypeDef* dmaHandle[3];
ADC_HandleTypeDef* adcHandle[3];

/**
 * This function handles DMA2 stream0 global interrupt. (For ADC 1)
 */
extern "C" void DMA2_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(dmaHandle[0]);
    HAL_ADC_IRQHandler(adcHandle[0]);
}

/**
 * This function handles DMA2 stream2 global interrupt. (For ADC 2)
 */
extern "C" void DMA2_Stream2_IRQHandler(void) {
    HAL_DMA_IRQHandler(dmaHandle[1]);
    HAL_ADC_IRQHandler(adcHandle[1]);
}

/**
 * This function handles DMA2 stream1 global interrupt. (For ADC 3)
 * This is correct, stream 1 for ADC 3. STM is odd and makes things confusing
 */
extern "C" void DMA2_Stream1_IRQHandler(void) {
    HAL_DMA_IRQHandler(dmaHandle[2]);
    HAL_ADC_IRQHandler(adcHandle[2]);
}
} // namespace

namespace core::io {
constexpr uint8_t ADC1_SLOT = 0;
constexpr uint8_t ADC2_SLOT = 1;
constexpr uint8_t ADC3_SLOT = 2;

bool ADCf4xx::timerInit = false;

ADCf4xx::ADCf4xx(Pin pin, ADCPeriph adcPeriph)
    : ADC(pin, adcPeriph), adcState(adcArray[getADCNum(adcPeriph)]), adcNum(getADCNum(adcPeriph)) {
    if (adcState.rank == MAX_CHANNELS) {
        log::LOGGER.log(log::Logger::LogLevel::WARNING, "ADC %d ALREADY HAS MAX NUMBER OF CHANNELS!!", (adcNum + 1));
        return;
    }

    if (timerInit) {
        HAL_TIM_Base_DeInit(&htim8); // Stop Timer8 (Trigger Source For ADC's)
        timerInit = false;
    }

    if (adcState.isADCInit) {
        HAL_ADC_Stop_DMA(&adcState.halADC);
        HAL_DMA_DeInit(&adcState.halDMA);
    } else {
        __HAL_RCC_DMA2_CLK_ENABLE();
        adcState.isADCInit = true;
    }

    initDMA();
    initADC(adcState.rank);
    addChannel(adcState.rank);

    dmaHandle[adcNum] = &this->adcArray[adcNum].halDMA;
    adcHandle[adcNum] = &this->adcArray[adcNum].halADC;

    if (!timerInit) {
        __HAL_RCC_TIM8_CLK_ENABLE();
        initTimer();
        HAL_TIM_Base_Start(&htim8); // Start Timer8 (Trigger Source For ADC's)
        timerInit = true;
    }

    HAL_ADC_Start_DMA(&adcState.halADC, reinterpret_cast<uint32_t*>(&adcState.buffer[0]), adcState.rank);
    adcState.rank++;
}

float ADCf4xx::read() {
    float percentage = readPercentage();
    return percentage * VREF_POS;
}

uint32_t ADCf4xx::readRaw() {
    uint8_t channelNum = 0;
    while (adcState.channels[channelNum] != pin) {
        channelNum++;
    }

    return adcState.buffer[channelNum];
}

float ADCf4xx::readPercentage() {
    float raw = static_cast<float>(readRaw());
    return static_cast<float>(raw / MAX_RAW);
}

void ADCf4xx::initADC(uint8_t num_channels) {
    /** Configure the global features of the ADC (Clock, Resolution, Data
     * Alignment and number of conversion)
     */
    ADC_HandleTypeDef* halADC = &adcState.halADC;
    // Set instance to the ADC peripheral being using
    halADC->Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
    halADC->Init.Resolution            = ADC_RESOLUTION_12B;
    halADC->Init.ScanConvMode          = ENABLE;
    halADC->Init.ContinuousConvMode    = DISABLE;
    halADC->Init.DiscontinuousConvMode = DISABLE;
    halADC->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    // Sets conversions to be done when timer 8 sends an Update Trigger
    halADC->Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO;
    halADC->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    halADC->Init.NbrOfConversion       = num_channels;
    halADC->Init.DMAContinuousRequests = ENABLE;
    halADC->Init.EOCSelection          = ADC_EOC_SINGLE_CONV;

    switch (adcNum) {
    case ADC1_SLOT:
        halADC->Instance = ADC1;
        __HAL_RCC_ADC1_CLK_ENABLE();
        break;
    case ADC2_SLOT:
        halADC->Instance = ADC2;
        __HAL_RCC_ADC2_CLK_ENABLE();
        break;
    case ADC3_SLOT:
        halADC->Instance = ADC3;
        __HAL_RCC_ADC3_CLK_ENABLE();
        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "INVALID ADC NUMBER!!");
        return; // Should never get here
    }
    HAL_ADC_Init(halADC);
}

void ADCf4xx::initDMA() {
    DMA_HandleTypeDef* dma = &adcState.halDMA;
    // Set DMA instance to proper config settings
    switch (adcNum) {
    case ADC1_SLOT:
        dma->Instance     = DMA2_Stream0;
        dma->Init.Channel = DMA_CHANNEL_0;
        break;
    case ADC2_SLOT:
        dma->Instance     = DMA2_Stream2;
        dma->Init.Channel = DMA_CHANNEL_1;
        break;
    case ADC3_SLOT:
        dma->Instance     = DMA2_Stream1;
        dma->Init.Channel = DMA_CHANNEL_2;
        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "INVALID ADC NUMBER!!");
        return; // Should never get here
    }
    dma->Init.Direction           = DMA_PERIPH_TO_MEMORY;
    dma->Init.PeriphInc           = DMA_PINC_DISABLE;
    dma->Init.MemInc              = DMA_MINC_ENABLE;
    dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    dma->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    dma->Init.Mode                = DMA_CIRCULAR;
    dma->Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    dma->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    dma->Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    dma->Init.MemBurst            = DMA_MBURST_SINGLE;
    dma->Init.PeriphBurst         = DMA_PBURST_SINGLE;

    HAL_DMA_Init(dma);

    switch (adcNum) {
    case ADC1_SLOT:
        HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, platform::ADC_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
        break;
    case ADC2_SLOT:
        HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, platform::ADC_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
        break;
    case ADC3_SLOT:
        HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, platform::ADC_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "INVALID ADC NUMBER!!");
        return; // Should never get here
    }
    __HAL_LINKDMA(&adcState.halADC, DMA_Handle, *dma);
}

void ADCf4xx::addChannel(uint8_t rank) {
    GPIO_InitTypeDef gpioInit;
    uint8_t numOfPins = 1;
    Pin pins[]        = {pin};

    GPIOf4xx::gpioStateInit(&gpioInit, pins, numOfPins, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

    ADC_ChannelConfTypeDef adcChannel;
    Channel_Support channelStruct;
    // Combines the ADC channel with the ADC peripherals it supports into a struct, avoiding having multi-layered switch
    // statements
    switch (pin) {
    case Pin::PA_0:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_0)};
        break;
    case Pin::PA_1:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_1)};
        break;
    case Pin::PA_2:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_2)};
        break;
    case Pin::PA_3:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_3)};
        break;
    case Pin::PA_4:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_4)};
        break;
    case Pin::PA_5:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_5)};
        break;
    case Pin::PA_6:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_6)};
        break;
    case Pin::PA_7:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_7)};
        break;
    case Pin::PB_0:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_8)};
        break;
    case Pin::PB_1:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_9)};
        break;
    case Pin::PC_0:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_10)};
        break;
    case Pin::PC_1:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_11)};
        break;
    case Pin::PC_2:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_12)};
        break;
    case Pin::PC_3:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 1, .channel = static_cast<uint8_t>(ADC_CHANNEL_13)};
        break;
    case Pin::PC_4:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_14)};
        break;
    case Pin::PC_5:
        channelStruct = {.adc1 = 1, .adc2 = 1, .adc3 = 0, .channel = static_cast<uint8_t>(ADC_CHANNEL_15)};
        break;
    default:
        channelStruct = {}; // sets all values to 0
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "INVALID PIN 0x%x!!", pin);
        break; // Should never get here
    }

    // This checks if the pin being used supports the ADC being used
    if (checkSupport(adcPeriph, channelStruct)) {
        adcChannel.Channel = channelStruct.channel;
    } else {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "ADC %d DOES NOT SUPPORT PIN 0x%x!!", (adcNum + 1), pin);
    }

    // Subtract 1 because rank starts at 1
    adcState.channels[rank - 1] = pin;

    adcChannel.Rank         = rank;
    adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    adcChannel.Offset       = 0;

    HAL_ADC_ConfigChannel(&adcState.halADC, &adcChannel);
}

bool ADCf4xx::checkSupport(ADCPeriph periph, Channel_Support channelStruct) {
    // In c++, non-zero values (like 1) are true, and 0 is false, so no comparison is needed.
    switch (periph) {
    case ADCPeriph::ONE:
        return channelStruct.adc1;
    case ADCPeriph::TWO:
        return channelStruct.adc2;
    case ADCPeriph::THREE:
        return channelStruct.adc3;
    }
}

uint8_t ADCf4xx::getADCNum(ADCPeriph periph) {
    switch (periph) {
    case ADCPeriph::ONE:
        return ADC1_SLOT;
    case ADCPeriph::TWO:
        return ADC2_SLOT;
    case ADCPeriph::THREE:
        return ADC3_SLOT;
    }
}

/*
 * This method initializes timer 8 with a Trigger Output of "Update Trigger", with a timer frequency of 1kHz.
 * The timer does not specifically tell the exact ADC to do a conversion, it just sends a general Update Trigger every
 * cycle. The ADC's are listening for this Update Trigger from timer 8, which is set in the ADC initialization.
 */
void ADCf4xx::initTimer() {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig     = {0};

    htim8.Instance               = TIM8;
    htim8.Init.Prescaler         = 0;
    htim8.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim8.Init.Period            = (SystemCoreClock / 1000) - 1;
    htim8.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim8.Init.RepetitionCounter = 0;
    htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htim8);
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig);
}

} // namespace core::io
