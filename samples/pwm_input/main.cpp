
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <core/utils/log.hpp>

/* Private defines -----------------------------------------------------------*/
#define B1_Pin             GPIO_PIN_13
#define B1_GPIO_Port       GPIOC
#define USART_TX_Pin       GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin       GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin            GPIO_PIN_13
#define LD2_GPIO_Port      GPIOB
#define TMS_Pin            GPIO_PIN_13
#define TMS_GPIO_Port      GPIOA
#define TCK_Pin            GPIO_PIN_14
#define TCK_GPIO_Port      GPIOA
#define SWO_Pin            GPIO_PIN_3
#define SWO_GPIO_Port      GPIOB

namespace io   = core::io;
namespace time = core::time;
namespace log  = core::log;

TIM_HandleTypeDef htim2;

static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

void Error_Handler(void);

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

    //setup logger
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();

    // io::PWM& pwm_input = io::getPWM<io::Pin::PA_0>();
    uart.printf("START     INPUT CAPTURE\n\r");
    // input capture

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
    __HAL_RCC_TIM2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA0     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* USER CODE END TIM2_Init 2 */
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "GPIO_INIT");
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin  = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LD2_Pin */
    GPIO_InitStruct.Pin   = LD2_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */

}

void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
        // TODO  Report Hal error return state

    }
    /* USER CODE END Error_Handler_Debug */
}
