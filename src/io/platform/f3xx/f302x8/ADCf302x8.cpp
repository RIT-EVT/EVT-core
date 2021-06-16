#include <EVT/io/platform/f3xx/f302x8/ADCf302x8.hpp>
#include <EVT/io/pin.hpp>

#include <HALf3/stm32f3xx_hal_adc.h>
#include <HALf3/stm32f3xx_hal_adc_ex.h>


namespace {
    /// This is made as a global variable so that it is accessible in the
    // interrupt.
    DMA_HandleTypeDef* DMA;

}  // namespace

extern "C" void DMA1_Channel1_IRQHandler() {
        HAL_DMA_IRQHandler(DMA);
}

namespace EVT::core::IO {

// Init static member variables
ADC_HandleTypeDef ADCf302x8::halADC = {0};
Pin ADCf302x8::channels[MAX_CHANNELS];
uint32_t ADCf302x8::buffer[MAX_CHANNELS];
DMA_HandleTypeDef ADCf302x8::halDMA = {0};


ADCf302x8::ADCf302x8(Pin pin) : ADC(pin) {

    // Flag representing if the ADC has been configured yet
    static bool halADCisInit = false;
    // "Rank" represents the channel value
    static uint8_t rank;

    if (!halADCisInit) {

        // Init ADC
        halADC.Instance = ADC1;
        halADC.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;     // TODO: Need a good value for this...seems to work for now
        halADC.Init.Resolution            = ADC_RESOLUTION_12B;           // 12-bit resolution
        halADC.Init.DataAlign             = ADC_DATAALIGN_RIGHT;          // LSB of data is located at bit 0
        halADC.Init.ScanConvMode          = ENABLE;                       // Conversions performed in sequence across all channels
        halADC.Init.EOCSelection          = DISABLE;                      // No end-of-conversion flag
        halADC.Init.LowPowerAutoWait      = DISABLE;                      // Don't transition to low-power state when not in use
        halADC.Init.ContinuousConvMode    = ENABLE;                       // Keep converting continuously after a trigger occurs (opposite of single mode / one conversion only)
        halADC.Init.NbrOfConversion       = 0;                            // Number of channels we're going to convert
        halADC.Init.DiscontinuousConvMode = DISABLE;                      // Perform all conversions in one complete sequence (as opposed to multiple discrete sequences)
        halADC.Init.NbrOfDiscConversion   = 1;                            // Number of subdivisions for the main conversion sequence
        halADC.Init.ExternalTrigConv      = ADC_SOFTWARE_START;           // Trigger manually with software
        halADC.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;// Don't care since we're using ADC_SOFTWARE_START as our trigger
        halADC.Init.DMAContinuousRequests = ENABLE;                       // Keep going once a full DMA transfer is done (opposite of single-shot mode)
        halADC.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;     // Overwrite the data in the DMA buffer upon overrun

        __HAL_RCC_ADC1_CLK_ENABLE();
        HAL_ADC_Init(&halADC);

        // Start DMA
        HAL_ADC_Stop(&halADC);

        DMA = &this->halDMA;

        __HAL_RCC_DMA1_CLK_ENABLE();

        this->halDMA.Instance                 = DMA1_Channel1;
        this->halDMA.Init.Direction           = DMA_PERIPH_TO_MEMORY;    // Data going from the ADC peripheral to a buffer in memory
        this->halDMA.Init.PeriphInc           = DMA_PINC_DISABLE;        // Disable peripheral pointer auto-increment
        this->halDMA.Init.MemInc              = DMA_MINC_ENABLE;         // Enable memory pointer auto-increment
        this->halDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // 16-bit data alignment (the ADC runs in 12-bit mode)
        this->halDMA.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        this->halDMA.Init.Mode                = DMA_CIRCULAR;            // Wrap around to the beginning of the buffer when we reach the end
        this->halDMA.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

        HAL_DMA_Init(&this->halDMA);
        HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

        __HAL_LINKDMA(&halADC, DMA_Handle, this->halDMA);

        HAL_ADC_Start_DMA(&halADC, &buffer[0], MAX_CHANNELS);

        halADCisInit = true;
    }

    // Add on the new channel
    halADC.Init.NbrOfConversion++;

    HAL_ADC_Init(&halADC);

    GPIO_InitTypeDef gpioInit;
    ADC_ChannelConfTypeDef adcChannel;

    // Configure GPIO for ADC
    gpioInit.Pin  = (static_cast<uint8_t>(pin) & 0x0F) + 1;
    gpioInit.Mode = GPIO_MODE_ANALOG;
    gpioInit.Pull = GPIO_NOPULL;

    // Which GPIO bank?
    switch ((static_cast<uint8_t>(pin) & 0xF0) >> 4)
    {
        case 0x0:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            HAL_GPIO_Init(GPIOA, &gpioInit);
            break;
        case 0x1:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            HAL_GPIO_Init(GPIOB, &gpioInit);
            break;
        case 0x2:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            HAL_GPIO_Init(GPIOC, &gpioInit);
            break;
        default:
            break; // Should never get here
    }

    switch (pin)
    {
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
            break; // Should never get here
    }

    // Subtract 1 because rank starts at 1
    channels[rank-1] = pin;

    adcChannel.Rank = rank++;
    adcChannel.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
    adcChannel.Offset = 0;

    HAL_ADC_ConfigChannel(&halADC, &adcChannel);

    HAL_DMA_Init(&this->halDMA);
    HAL_ADC_Start_DMA(&halADC, &buffer[0], MAX_CHANNELS);

}

float ADCf302x8::read() {
    float percentage = readPercentage();
    return percentage * MAX_VOLTAGE;
}

uint32_t ADCf302x8::readRaw() {
    // Search through list of channels to determine which DMA buffer index to
    // use
    uint8_t channelNum = 0;
    while(channels[channelNum] != pin)
        channelNum++;
    return static_cast<float>(buffer[channelNum]);
}

float ADCf302x8::readPercentage() {
    uint32_t raw = readRaw();
    return static_cast<float>(raw / MAX_RAW);
}

}  // namespace EVT::core::IO
