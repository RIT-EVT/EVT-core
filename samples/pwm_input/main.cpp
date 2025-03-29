
// Xander  2/3/2025.
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;
namespace log  = core::log;

// Defines for testing ---------------------------------------------------------------------------------
//Channel defines-----
#define directChannel       TIM_CHANNEL_2                 // TIM_CHANNEL_1
#define activeDirectChannel HAL_TIM_ACTIVE_CHANNEL_1     // HAL_TIM_ACTIVE_CHANNEL_1
#define indirectChannel     TIM_CHANNEL_2                 // TIM_CHANNEL_2
#define triggerSource       TIM_TS_TI2FP2                 // TIM_TS_TI1FP1
#define gpioClkEnable       __HAL_RCC_GPIOA_CLK_ENABLE(); //__HAL_RCC_GPIOA_CLK_ENABLE();
#define thePin              GPIO_PIN_3                    // GPIO_PIN_0
#define altFunc             GPIO_AF9_TIM15                 // GPIO_AF1_TIM2
#define gpioType            GPIOA                           // GPIOA
//Timer Defines-----
#define TIMNum              TIM15                            //TIM  2
#define IRQHandler          TIM1_BRK_TIM15_IRQHandler        //TIM2_IRQHandler
#define htimNum             htim15                         //htim2
#define IRQNum              TIM1_BRK_TIM15_IRQn                    //TIM2_IRQn
#define clkEnable           __HAL_RCC_TIM15_CLK_ENABLE();  //__HAL_RCC_TIM2_CLK_ENABLE();
//-------------------------------------------------------------------------------------------------------

TIM_HandleTypeDef htimNum;



static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

void Error_Handler(void);

// IRQ handler for TIM2
extern "C" void IRQHandler (void) {
    HAL_TIM_IRQHandler(&htimNum);
}

uint32_t ICValue   = 0;
uint32_t Frequency = 0;
uint32_t Duty      = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "CAPTURE_CALLBACK!!");
    if (htim->Channel == activeDirectChannel) // If the interrupt is triggered by channel 1
    {
        // Read the IC value
        ICValue = HAL_TIM_ReadCapturedValue(htim, directChannel);

        if (ICValue != 0) // first value is zero, non zero is period
        {
            // calculate the Duty Cycle
            // signal high time/ period = duty cycle
            Duty = (HAL_TIM_ReadCapturedValue(htim, indirectChannel) * 100) / ICValue;

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
    HAL_NVIC_SetPriority(IRQNum, 0, 0);
    HAL_NVIC_EnableIRQ(IRQNum);
    // enable interrupt src
    HAL_TIM_IC_Start_IT(&htimNum, directChannel); // main channel
    HAL_TIM_IC_Start(&htimNum, indirectChannel);  // indirect channel

    uart.printf("END       INPUT CAPTURE\n\r");
    while (1) {
        time::wait(1000);
        uart.printf("\n\rICValue: %d\n\r", ICValue);
        uart.printf("\n\rFrequency: %d\n\r", Frequency) ;
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
    //__HAL_RCC_TIM2_CLK_ENABLE();
    clkEnable;
    /* USER CODE END TIM2_Init 1 */
    htimNum.Instance               = TIMNum;
    htimNum.Init.Prescaler         = 0;
    htimNum.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htimNum.Init.Period            = 4294967295;
    htimNum.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htimNum.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htimNum) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htimNum, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htimNum) != HAL_OK) {
        Error_Handler();
    }
    sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger     = triggerSource;
    sSlaveConfig.TriggerPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
    sSlaveConfig.TriggerFilter    = 0;
    if (HAL_TIM_SlaveConfigSynchro(&htimNum, &sSlaveConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htimNum, &sConfigIC, directChannel) != HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&htimNum, &sConfigIC, indirectChannel) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htimNum, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    //__HAL_RCC_GPIOA_CLK_ENABLE();
    gpioClkEnable
        /**TIM2 GPIO Configuration
        PA0     ------> TIM2_CH1
        */

        GPIO_InitStruct.Pin   = thePin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = altFunc;
    HAL_GPIO_Init(gpioType, &GPIO_InitStruct);
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
