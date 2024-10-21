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
 * For commit from before code was removed, refer to commit 81624521a8b2c4b66480193e88cf32782aaee84d.
 */

#include <HALf4/stm32f4xx.h>
#include <HALf4/stm32f4xx_hal_adc.h>
#include <core/io/pin.hpp>
#include <core/io/platform/f4xx/ADCf4xx.hpp>
#include <core/io/platform/f4xx/GPIOf4xx.hpp>

namespace core::io {
#define ADC1_SLOT 0
#define ADC2_SLOT 1
#define ADC3_SLOT 2

#define ADC1SHIFT 5
#define ADC2SHIFT 6
#define ADC3SHIFT 7

// Combines the channel memory value with the ADC peripherals it supports into one uint32_t
#define CHANNEL_SET(adc1, adc2, adc3, ch) (ch | (adc1 << ADC1SHIFT) | (adc2 << ADC2SHIFT) | (adc3 << ADC3SHIFT))

ADCf4xx::ADC_State_t ADCf4xx::adcArray[3];
uint8_t ADCf4xx::rank      = 1;
uint16_t ADCf4xx::buffer[] = {0};

ADCf4xx::ADCf4xx(Pin pin, ADCPeriph adcPeriph) : ADC(pin, adcPeriph) {

    ADCf4xx::ADC_State_t* adcState = &adcArray[getADCNum()];

    if (rank == MAX_CHANNELS) {
        return;
    }

    initADC(rank);
    addChannel(rank);

    HAL_ADC_Start(&adcState->halADC);
    rank++;
}

float ADCf4xx::read() {
    float percentage = readPercentage();
    return percentage * VREF_POS;
}

uint32_t ADCf4xx::readRaw() {
    ADCf4xx::ADC_State_t* adcState = &adcArray[getADCNum()];

    HAL_ADC_PollForConversion(&adcState->halADC, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&adcState->halADC);
}

float ADCf4xx::readPercentage() {
    float raw = static_cast<float>(readRaw());
    return static_cast<float>(raw / MAX_RAW);
}

void ADCf4xx::initADC(uint8_t num_channels) {
    /** Configure the global features of the ADC (Clock, Resolution, Data
     * Alignment and number of conversion)
     */
    ADCf4xx::ADC_State_t* adcState = &adcArray[getADCNum()];
    ADC_HandleTypeDef* halADC      = &adcState->halADC;
    // Set instance to the ADC peripheral being using
    halADC->Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
    halADC->Init.Resolution            = ADC_RESOLUTION_12B;
    halADC->Init.ScanConvMode          = ENABLE;
    halADC->Init.ContinuousConvMode    = ENABLE;
    halADC->Init.DiscontinuousConvMode = DISABLE;
    halADC->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    halADC->Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    halADC->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    halADC->Init.NbrOfConversion       = num_channels;
    halADC->Init.DMAContinuousRequests = DISABLE;
    halADC->Init.EOCSelection          = ADC_EOC_SEQ_CONV;

    switch (getADCNum()) {
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
        return; // Should never get here
    }
    HAL_ADC_Init(halADC);
}

void ADCf4xx::initDMA() {
    uint8_t adcNum                 = getADCNum();
    ADCf4xx::ADC_State_t* adcState = &adcArray[adcNum];
    DMA_HandleTypeDef* dma         = &adcState->halDMA;
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

    HAL_DMA_Init(dma);
    __HAL_LINKDMA(&adcState->halADC, DMA_Handle, *dma);
}

void ADCf4xx::addChannel(uint8_t rank) {
    GPIO_InitTypeDef gpioInit;
    uint8_t numOfPins = 1;
    uint32_t channel;
    Pin myPins[]                   = {pin};
    ADCf4xx::ADC_State_t* adcState = &adcArray[getADCNum()];

    GPIOf4xx::gpioStateInit(&gpioInit, myPins, numOfPins, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

    ADC_ChannelConfTypeDef adcChannel;

    // Combines the ADC channel with the ADC peripherals it supports, to avoid having multi-layered switch statements
    switch (pin) {
    case Pin::PA_0:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_0);
        break;
    case Pin::PA_1:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_1);
        break;
    case Pin::PA_2:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_2);
        break;
    case Pin::PA_3:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_3);
        break;
    case Pin::PA_4:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_4);
        break;
    case Pin::PA_5:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_5);
        break;
    case Pin::PA_6:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_6);
        break;
    case Pin::PA_7:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_7);
        break;
    case Pin::PB_0:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_8);
        break;
    case Pin::PB_1:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_9);
        break;
    case Pin::PC_0:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_10);
        break;
    case Pin::PC_1:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_11);
        break;
    case Pin::PC_2:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_12);
        break;
    case Pin::PC_3:
        channel = CHANNEL_SET(1, 1, 1, ADC_CHANNEL_13);
        break;
    case Pin::PC_4:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_14);
        break;
    case Pin::PC_5:
        channel = CHANNEL_SET(1, 1, 0, ADC_CHANNEL_15);
        break;
    default:
        channel = 0;
        break; // Should never get here
    }

    // This checks if the pin being used supports the ADC being used
    if (checkSupport(adcPeriph, channel)) {
        // Masks channel back to proper value (Zero's out ADC information bits)
        adcChannel.Channel = channel & 0x1F;
    } else {
        // Causes HARD FAULT if pin does not support the ADC peripheral being used. THIS IS INTENTIONAL!
        adcChannel.Channel = *((uint32_t*) 0U);
    }

    // Subtract 1 because rank starts at 1
    adcState->channels[rank - 1] = pin;

    adcChannel.Rank         = rank;
    adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    adcChannel.Offset       = 0;
    adcChannel.Offset       = 0x000;

    HAL_ADC_ConfigChannel(&adcState->halADC, &adcChannel);
}

inline bool ADCf4xx::checkSupport(ADCPeriph periph, uint32_t channel) {
    // Checks if the channel contains the bit signifying the proper ADC peripheral support
    switch (periph) {
    case ADCPeriph::ONE:
        return channel & (1 << ADC1SHIFT);
    case ADCPeriph::TWO:
        return channel & (1 << ADC2SHIFT);
    case ADCPeriph::THREE:
        return channel & (1 << ADC3SHIFT);
    }
}

uint8_t ADCf4xx::getADCNum() {
    switch (adcPeriph) {
    case ADCPeriph::ONE:
        return ADC1_SLOT;
    case ADCPeriph::TWO:
        return ADC2_SLOT;
    case ADCPeriph::THREE:
        return ADC3_SLOT;
    }
}

} // namespace core::io
