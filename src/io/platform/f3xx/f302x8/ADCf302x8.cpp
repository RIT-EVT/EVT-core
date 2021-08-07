#include <EVT/io/platform/f3xx/f302x8/ADCf302x8.hpp>
#include <EVT/io/pin.hpp>

#include <HALf3/stm32f3xx.h>

#include <HALf3/stm32f3xx_hal_adc.h>
#include <HALf3/stm32f3xx_hal_adc_ex.h>


namespace {
    /// This is made as a global variable so that it is accessible in the
    // interrupt.
    DMA_HandleTypeDef* dmaHandle;
    ADC_HandleTypeDef* adcHandle;

}  // namespace

extern "C" void DMA1_Channel1_IRQHandler(void) {
    HAL_DMA_IRQHandler(dmaHandle);
}

/**
 * Current "work around" solution that ensurs the clock settings are applied
 * and the HAL is initialized. This will later be moved into a common place
 * that will happen once at system start up. For the time being this can be
 * used for basic setup. This is not a long term solution.
 */
static void lowLevelInit() {
    HAL_Init();
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC1;
    PeriphClkInit.Adc1ClockSelection = RCC_ADC1PLLCLK_DIV1;

    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
}

namespace EVT::core::IO {

// Init static member variables
ADC_HandleTypeDef ADCf302x8::halADC = {0};
Pin ADCf302x8::channels[MAX_CHANNELS];
uint16_t ADCf302x8::buffer[MAX_CHANNELS];
DMA_HandleTypeDef ADCf302x8::halDMA = {0};


ADCf302x8::ADCf302x8(Pin pin) : ADC(pin) {
    // Flag representing if the ADC has been configured yet
    static bool halADCisInit = false;
    // "Rank" represents the order in which the channels are added
    static uint8_t rank = 1;

    // Initialization of the HAL ADC should only take place once since there is
    // only one ADC device which has muliple channels supported.
    if (!halADCisInit) {
        __HAL_RCC_DMA1_CLK_ENABLE();

        lowLevelInit();
        initADC();

        initDMA();

        dmaHandle = &this->halDMA;
        adcHandle = &this->halADC;

        halADCisInit = true;
    }

    addChannel(rank);
    initDMA();

    HAL_ADC_Start_DMA(&halADC, reinterpret_cast<uint32_t*>(&buffer[0]),
            MAX_CHANNELS);

    rank++;
}

float ADCf302x8::read() {
    float percentage = readPercentage();
    return percentage * MAX_VOLTAGE;
}

uint32_t ADCf302x8::readRaw() {
    // Search through list of channels to determine which DMA buffer index to
    // use
    uint8_t channelNum = 0;
    while (channels[channelNum] != pin)
        channelNum++;
    return buffer[channelNum];
}

float ADCf302x8::readPercentage() {
    float raw = static_cast<float>(readRaw());
    return static_cast<float>(raw / MAX_RAW);
}

void ADCf302x8::initADC() {
    halADC.Instance = ADC1;     // Only ADC the F3 supportes

    halADC.Init.ClockPrescaler          = ADC_CLOCK_ASYNC_DIV1;
    halADC.Init.Resolution              = ADC_RESOLUTION_12B;
    halADC.Init.DataAlign               = ADC_DATAALIGN_RIGHT;
    halADC.Init.ScanConvMode            = ADC_SCAN_ENABLE;
    halADC.Init.EOCSelection            = DISABLE;
    halADC.Init.LowPowerAutoWait        = DISABLE;
    halADC.Init.ContinuousConvMode      = ENABLE;
    halADC.Init.NbrOfConversion         = MAX_CHANNELS;
    halADC.Init.DiscontinuousConvMode   = DISABLE;
    halADC.Init.NbrOfDiscConversion     = 1;
    halADC.Init.ExternalTrigConv        = ADC_SOFTWARE_START;
    halADC.Init.ExternalTrigConvEdge    = ADC_EXTERNALTRIGCONVEDGE_NONE;
    halADC.Init.DMAContinuousRequests   = ENABLE;
    halADC.Init.Overrun                 = ADC_OVR_DATA_OVERWRITTEN;

    __HAL_RCC_ADC1_CLK_ENABLE();
    HAL_ADC_Init(&halADC);
}

void ADCf302x8::initDMA() {
    // HAL_ADC_Stop(&halADC);

    // TODO: Add some way of selecting the next available DMA channel
    // Ideally we would have a "DMA" class dedicated to DMA resource
    // allocation.
    halDMA.Instance                 = DMA1_Channel1;
    halDMA.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    halDMA.Init.PeriphInc           = DMA_PINC_DISABLE;
    halDMA.Init.MemInc              = DMA_MINC_ENABLE;
    halDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    halDMA.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    halDMA.Init.Mode                = DMA_CIRCULAR;
    halDMA.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

    HAL_DMA_Init(&halDMA);

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    __HAL_LINKDMA(&halADC, DMA_Handle, halDMA);
}

void ADCf302x8::addChannel(uint8_t rank) {
    GPIO_InitTypeDef gpioInit;
    ADC_ChannelConfTypeDef adcChannel;

    // Configure GPIO for ADC
    gpioInit.Pin  = (static_cast<uint8_t>(pin) & 0x0F) + 1;
    gpioInit.Mode = GPIO_MODE_ANALOG;
    gpioInit.Pull = GPIO_NOPULL;

    // Which GPIO bank?
    switch ((static_cast<uint8_t>(pin) & 0xF0) >> 4) {
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
            break;  // Should never get here
    }

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
            break;  // Should never get here
    }

    // Subtract 1 because rank starts at 1
    channels[rank-1] = pin;

    adcChannel.Rank = rank;
    adcChannel.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
    adcChannel.Offset = 0;

    HAL_ADC_ConfigChannel(&halADC, &adcChannel);
}

}  // namespace EVT::core::IO