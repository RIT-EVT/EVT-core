
// Xander  2/3/2025.

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;
namespace log  = core::log;

TIM_HandleTypeDef htim2;

static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

void Error_Handler(void);

// IRQ handler for TIM2
extern "C" void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

uint32_t ICValue   = 0;
uint32_t Frequency = 0;
uint32_t Duty      = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "CAPTURE_CALLBACK!!");
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // If the interrupt is triggered by channel 1
    {
        // Read the IC value
        ICValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

        if (ICValue != 0) // first value is zero, non zero is period
        {
            // calculate the Duty Cycle
            // signal high time/ period = duty cycle
            Duty = (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) * 100) / ICValue;

            Frequency = SystemCoreClock / ICValue; // APB1 Timer clocks/period = F
        }
    }
}

int main(void) {
    // Initialize system
    core::platform::init();
    // Setup UART
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // setup logger
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    /* Initialize all configured peripherals */
    MX_TIM2_Init();

    uart.printf("START     INPUT CAPTURE\n\r");
    // input capture
    // enable TIM2 interrupt, set priority
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    // enable interrupt src
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // main channel
    HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2);    // indirect channel

    uart.printf("END       INPUT CAPTURE\n\r");
    while (1) {
        time::wait(1000);
        uart.printf("\n\rICValue: %d\n\r", ICValue);
        uart.printf("\n\rFrequency: %d\n\r", Frequency);
        uart.printf("\n\rDuty: %d\n\r", Duty);
        uart.printf("\n\r----------------------------------\n\r");
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */

static void MX_TIM2_Init(void) {
    /* USER CODE BEGIN TIM2_Init 0 */
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "TIM2_INIT");
    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig       = {0};
    TIM_IC_InitTypeDef sConfigIC              = {0};
    TIM_MasterConfigTypeDef sMasterConfig     = {0};

    /* USER CODE BEGIN TIM2_Init 1 */
    __HAL_RCC_TIM2_CLK_ENABLE();
    /* USER CODE END TIM2_Init 1 */
    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 0;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 4294967295;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger     = TIM_TS_TI1FP1;
    sSlaveConfig.TriggerPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
    sSlaveConfig.TriggerFilter    = 0;
    if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA0     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin       = GPIO_PIN_0;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* USER CODE END TIM2_Init 2 */
}

void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
        // TODO  Report Hal error return state
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Bad");
    }
    /* USER CODE END Error_Handler_Debug */
}
