/**
 * DISCLAIMER: THIS MIGHT BREAK AT ANY POINT AND/OR NOT WORK FOR CERTAIN PURPOSES
 *
 * This DMA ADC is different than f3xx DMA ADC!
 * This DMA ADC does NOT use the interrupts, as when interrupts were enabled it
 * would constantly interrupt, not letting the print statements or anything
 * else happen.
 * Tried to fix this by changing the sampletime and clock prescaler values, but
 * nothing worked.
 * This was "fixed" by commenting out the NVIC_EnableIRQ, stopping the interrupt
 * from ever being enabled.
 *
 * For commit from before code was removed, refer to THIS commit.
 */

#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f4xx/ADCf4xx.hpp>
#include <EVT/io/platform/f4xx/GPIOf4xx.hpp>
#include <HALf4/stm32f4xx.h>
#include <HALf4/stm32f4xx_hal_adc.h>

namespace {
/// This is made as a global variable so that it is accessible in the
/// interrupt.
DMA_HandleTypeDef* dmaHandle;
ADC_HandleTypeDef* adcHandle;

}// namespace

extern "C" void DMA2_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(dmaHandle);  // todo: just so everyone knows, this is here, but im 90% sure it isn't used... -travis :)
    HAL_ADC_IRQHandler(adcHandle);  // todo: was here in f3xx, everything i've found doesn't have this line for f4xx. -travis :)
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
        __HAL_RCC_DMA2_CLK_ENABLE();
        halADCisInit = true;
    }

    dmaHandle = &this->halDMA;
    adcHandle = &this->halADC;

    addChannel(rank);

    initADC(rank);

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
    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
      */
    halADC.Instance = ADC1;
    halADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    halADC.Init.Resolution = ADC_RESOLUTION_12B;
    halADC.Init.ScanConvMode = ENABLE;  // todo: this mode is DISABLE in all f4xx dma adc examples i found online, but needs to be ENABLE for it to work rn
    halADC.Init.ContinuousConvMode = ENABLE;
    halADC.Init.DiscontinuousConvMode = DISABLE;
    halADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    halADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    halADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    halADC.Init.NbrOfConversion = num_channels;
    halADC.Init.DMAContinuousRequests = ENABLE;
    halADC.Init.EOCSelection = ADC_EOC_SEQ_CONV;

    __HAL_RCC_ADC1_CLK_ENABLE();
    HAL_ADC_Init(&halADC);
}

void ADCf4xx::initDMA() {
    //     HAL_ADC_Stop(&halADC); // was commented out in f3xx...

    halDMA.Instance = DMA2_Stream0;
    halDMA.Init.Direction = DMA_PERIPH_TO_MEMORY;
    halDMA.Init.PeriphInc = DMA_PINC_DISABLE;
    halDMA.Init.MemInc = DMA_MINC_ENABLE;
    halDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    halDMA.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    halDMA.Init.Mode = DMA_CIRCULAR;
    halDMA.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    halDMA.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    HAL_DMA_Init(&halDMA);
    __HAL_LINKDMA(&halADC, DMA_Handle, halDMA);

    /* DMA interrupt init */
    /* DMA2_Stream0_IRQn interrupt configuration */
//    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, EVT::core::platform::ADC_INTERRUPT_PRIORITY, 0);
//    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn); // todo: look into this being unnecessary (isn't needed for working adc rn) -travis :)
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
        adcChannel.Channel = ADC_CHANNEL_0;
        break;
    case Pin::PA_1:
        adcChannel.Channel = ADC_CHANNEL_1;
        break;
    case Pin::PA_2:
        adcChannel.Channel = ADC_CHANNEL_2;
        break;
    case Pin::PA_3:
        adcChannel.Channel = ADC_CHANNEL_3;
        break;
    case Pin::PA_4:
        adcChannel.Channel = ADC_CHANNEL_4;
        break;
    case Pin::PA_5:
        adcChannel.Channel = ADC_CHANNEL_5;
        break;
    case Pin::PA_6:
        adcChannel.Channel = ADC_CHANNEL_6;
        break;
    case Pin::PA_7:
        adcChannel.Channel = ADC_CHANNEL_7;
        break;
    case Pin::PB_0:
        adcChannel.Channel = ADC_CHANNEL_8;
        break;
    case Pin::PB_1:
        adcChannel.Channel = ADC_CHANNEL_9;
        break;
    case Pin::PC_0:
        adcChannel.Channel = ADC_CHANNEL_10;
        break;
    case Pin::PC_1:
        adcChannel.Channel = ADC_CHANNEL_11;
        break;
    case Pin::PC_2:
        adcChannel.Channel = ADC_CHANNEL_12;
        break;
    case Pin::PC_3:
        adcChannel.Channel = ADC_CHANNEL_13;
        break;
    case Pin::PC_4:
        adcChannel.Channel = ADC_CHANNEL_14;
        break;
    case Pin::PC_5:
        adcChannel.Channel = ADC_CHANNEL_15;
        break;
    default:
        break;  // Should never get here
    }

    // Subtract 1 because rank starts at 1
    channels[rank - 1] = pin;

    adcChannel.Rank = rank;
    adcChannel.SamplingTime = ADC_SAMPLETIME_480CYCLES; // todo: F3xx was ADC_SAMPLETIME_601CYCLES_5. Doesn't exist in F4xx -travis :)
    adcChannel.Offset = 0;
    adcChannel.Offset = 0x000;

    HAL_ADC_ConfigChannel(&halADC, &adcChannel);
}

/*
    // todo: for the current code this isn't necessary ig? -travis :)
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* halADC) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* halADC) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

*/

}// namespace EVT::core::IO