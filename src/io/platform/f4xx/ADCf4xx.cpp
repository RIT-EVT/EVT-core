#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f4xx/ADCf4xx.hpp>
#include <EVT/io/platform/f4xx/GPIOf4xx.hpp>

#include <HALf4/stm32f4xx.h>

#include <EVT/platform/f4xx/stm32f4xx.hpp>
#include <HALf4/stm32f4xx_hal_adc.h>
#include <HALf4/stm32f4xx_hal_adc_ex.h>

namespace {
/// This is made as a global variable so that it is accessible in the
// interrupt.
DMA_HandleTypeDef* dmaHandle;
ADC_HandleTypeDef* adcHandle;

}// namespace

extern "C" void DMA1_Channel1_IRQHandler(void) {
    HAL_DMA_IRQHandler(dmaHandle);
    HAL_ADC_IRQHandler(adcHandle);
}

namespace EVT::core::IO {

// Init static member variables
ADC_HandleTypeDef ADCf4xx::halADC = {0};
Pin ADCf4xx::channels[MAX_CHANNELS];
uint16_t ADCf4xx::buffer[MAX_CHANNELS];
DMA_HandleTypeDef ADCf4xx::halDMA = {0};

ADCf4xx::ADCf4xx(Pin pin) : ADC(pin) {
    // Flag representing if the ADC has been configured yet
    static bool halADCisInit = false;
    // "Rank" represents the order in which the channels are added
    // Also represents the total number of added channels
    static uint8_t rank = 1;

    // Maximum number of ADC channels have already been added
    if (rank == MAX_CHANNELS) {
        return;
    }

    // Initialization of the HAL ADC should only take place once since there is
    // only one ADC device which has multiple channels supported.
    if (halADCisInit) {
        HAL_ADC_Stop_DMA(&halADC);
        HAL_DMA_DeInit(&halDMA);
    } else {
        __HAL_RCC_DMA1_CLK_ENABLE();
        halADCisInit = true;
    }
    initADC(rank);

    dmaHandle = &this->halDMA;
    adcHandle = &this->halADC;

    addChannel(rank);

    initDMA();
    HAL_ADC_Start_DMA(&halADC, reinterpret_cast<uint32_t*>(&buffer[0]),
                      rank);

    rank++;
}

float ADCf4xx::read() {
    float percentage = readPercentage();
    return percentage * VREF_POS;
}

uint32_t ADCf4xx::readRaw() {
    // Search through list of channels to determine which DMA buffer index to
    // use
    uint8_t channelNum = 0;
    while (channels[channelNum] != pin)
        channelNum++;
    return buffer[channelNum];
}

float ADCf4xx::readPercentage() {
    float raw = static_cast<float>(readRaw());
    return static_cast<float>(raw / MAX_RAW);
}

void ADCf4xx::initADC(uint8_t num_channels) {
    halADC.Instance = ADC1;// Only ADC the F3 supports

    // TODO: Figure out ADC calibration

    halADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; // Use AHB clock (8MHz) w/o division for ADC clock
    halADC.Init.Resolution = ADC_RESOLUTION_12B;
    halADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    halADC.Init.ScanConvMode = ENABLE;
    halADC.Init.EOCSelection = ADC_EOC_SEQ_CONV;
//    halADC.Init.LowPowerAutoWait = DISABLE;// Wait for the previous value to be written by DMA before beginning
                                           // next transfer.  Not recommended for DMA. DOES NOT EXIST IN F4xx
    halADC.Init.ContinuousConvMode = ENABLE;
    halADC.Init.NbrOfConversion = num_channels;
    halADC.Init.DiscontinuousConvMode = DISABLE;
    halADC.Init.NbrOfDiscConversion = 1;// Parameter discarded when Discontinuous Conv Mode is Disabled
    halADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    halADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;// Parameter discared when set to ADC_SOFTWARE_START
    halADC.Init.DMAContinuousRequests = ENABLE;
//    halADC.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; // DOES NOT EXIST IN F4xx

    __HAL_RCC_ADC1_CLK_ENABLE();
    HAL_ADC_Init(&halADC);
}

void ADCf4xx::initDMA() {
    // HAL_ADC_Stop(&halADC);

    // TODO: Add some way of selecting the next available DMA channel
    // Ideally we would have a "DMA" class dedicated to DMA resource
    // allocation.
    halDMA.Instance = DMA1_Stream1; // OG DMA1_Channel1, doesnt exist. stream is pretty close to channel :)
    halDMA.Init.Direction = DMA_PERIPH_TO_MEMORY;
    halDMA.Init.PeriphInc = DMA_PINC_DISABLE;
    halDMA.Init.MemInc = DMA_MINC_ENABLE;
    halDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    halDMA.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    halDMA.Init.Mode = DMA_CIRCULAR;
    halDMA.Init.Priority = DMA_PRIORITY_VERY_HIGH;

    HAL_DMA_Init(&halDMA);

    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, EVT::core::platform::ADC_INTERRUPT_PRIORITY, 0); // OG DMA1_Channel1_IRQn, doesnt exist. channel and stream are pretty close right :)
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn); // OG DMA1_Channel1_IRQn, doesnt exist. channel and stream are pretty close right :)

    __HAL_LINKDMA(&halADC, DMA_Handle, halDMA);
}

void ADCf4xx::addChannel(uint8_t rank) {
    GPIO_InitTypeDef gpioInit;
    Pin myPins[] = {pin};
    uint8_t numOfPins = 1;

    GPIOf4xx::gpioStateInit(&gpioInit, myPins, numOfPins, GPIO_MODE_ANALOG,
                            GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

    ADC_ChannelConfTypeDef adcChannel;

    switch (pin) {
    case Pin::PA_0:
        adcChannel.Channel = ADC_CHANNEL_1;
        break;
    case Pin::PA_1:
        adcChannel.Channel = ADC_CHANNEL_2;
        break;
    case Pin::PA_2:
        adcChannel.Channel = ADC_CHANNEL_3;
        break;
    case Pin::PA_3:
        adcChannel.Channel = ADC_CHANNEL_4;
        break;
    case Pin::PA_4:
        adcChannel.Channel = ADC_CHANNEL_5;
        break;
    case Pin::PC_0:
        adcChannel.Channel = ADC_CHANNEL_6;
        break;
    case Pin::PC_1:
        adcChannel.Channel = ADC_CHANNEL_7;
        break;
    case Pin::PC_2:
        adcChannel.Channel = ADC_CHANNEL_8;
        break;
    case Pin::PC_3:
        adcChannel.Channel = ADC_CHANNEL_9;
        break;
    case Pin::PA_6:
        adcChannel.Channel = ADC_CHANNEL_10;
        break;
    case Pin::PB_0:
        adcChannel.Channel = ADC_CHANNEL_11;
        break;
    case Pin::PB_1:
        adcChannel.Channel = ADC_CHANNEL_12;
        break;
    case Pin::PB_13:
        adcChannel.Channel = ADC_CHANNEL_13;
        break;
    case Pin::PB_11:
        adcChannel.Channel = ADC_CHANNEL_14;
        break;
    case Pin::PA_7:
        adcChannel.Channel = ADC_CHANNEL_15;
        break;
    default:
        break;// Should never get here
    }

    // Subtract 1 because rank starts at 1
    channels[rank - 1] = pin;

    adcChannel.Rank = rank;
    adcChannel.SamplingTime = ADC_SAMPLETIME_480CYCLES; // OG ADC_SAMPLETIME_601CYCLES_5. DOESNT EXIST
    adcChannel.Offset = 0;
//    adcChannel.SingleDiff = ADC_SINGLE_ENDED;     //  Not recommended for DMA. DOES NOT EXIST IN F4xx
//    adcChannel.OffsetNumber = ADC_OFFSET_NONE;    // Not recommended for DMA. DOES NOT EXIST IN F4xx
    adcChannel.Offset = 0x000;

    HAL_ADC_ConfigChannel(&halADC, &adcChannel);
}

}// namespace EVT::core::IO
