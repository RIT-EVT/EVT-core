#include <EVT/io/platform/f3xx/f302x8/PWMf302x8.hpp>

namespace EVT::core::IO {

/**
 * Startup the system clock
 */
static void systemClockInit() {
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM16;
  PeriphClkInit.Tim16ClockSelection = RCC_TIM16CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
  }

}

/**
 * Get the TIM instance and channel associated with a pin. This information
 * is pulled from the STM32F302x8 documentation. Easier representation of this
 * data can be found on MBed's STM32F302r8 documentation.
 *
 * @param pin The pin to check the instance of
 * @param instance The instance to assign to
 * @param channel The channel to assign to
 * @param alternateFunction The GPIO identifier for the function of the pin
 */
static void getInstance(Pin pin, TIM_TypeDef** instance, uint16_t* channel,
        uint32_t* alternateFunction) {
    switch (pin) {
        // complementary channel
        case Pin::PA_1:
            *instance = TIM15;
            *channel = TIM_CHANNEL_1;
            *alternateFunction = GPIO_AF9_TIM15;
            break;
        case Pin::PA_2:
            *instance = TIM15;
            *channel = TIM_CHANNEL_1;
            *alternateFunction = GPIO_AF9_TIM15;
            break;
        case Pin::PA_3:
            *instance = TIM15;
            *channel = TIM_CHANNEL_2;
            *alternateFunction = GPIO_AF9_TIM15;
            break;
        case Pin::PA_8:
            *instance = TIM1;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PA_6:
            *instance = TIM16;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PA_7:
            *instance = TIM17;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PA_9:
            *instance = TIM1;
            *channel = TIM_CHANNEL_2;
            break;
        case Pin::PA_10:
            *instance = TIM1;
            *channel = TIM_CHANNEL_3;
            break;
        case Pin::PA_11:
            *instance = TIM1;
            *channel = TIM_CHANNEL_4;
            break;
        case Pin::PA_12:
            *instance = TIM1;
            *channel = TIM_CHANNEL_1;
            break;
        // complementary channel
        case Pin::PA_13:
            *instance = TIM16;
            *channel = TIM_CHANNEL_1;
            break;
        // complementary channel
        case Pin::PB_0:
            *instance = TIM1;
            *channel = TIM_CHANNEL_2;
            break;
        //complementary channel
        case Pin::PB_1:
            *instance = TIM1;
            *channel = TIM_CHANNEL_3;
            break;
        case Pin::PB_4:
            *instance = TIM16;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PB_5:
            *instance = TIM17;
            *channel = TIM_CHANNEL_1;
            break;
        // complementary channel
        case Pin::PB_6:
            *instance = TIM16;
            *channel = TIM_CHANNEL_1;
            break;
        // complementary channel
        case Pin::PB_7:
            *instance = TIM17;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PB_8:
            *instance = TIM16;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PB_9:
            *instance = TIM17;
            *channel = TIM_CHANNEL_1;
            break;
        // complementation channel
        case Pin::PB_13:
            *instance = TIM1;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PB_14:
            *instance = TIM15;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PB_15:
            *instance = TIM15;
            *channel = TIM_CHANNEL_2;
            break;
        case Pin::PC_0:
            *instance = TIM1;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PC_1:
            *instance = TIM1;
            *channel = TIM_CHANNEL_2;
            break;
        case Pin::PC_2:
            *instance = TIM1;
            *channel = TIM_CHANNEL_3;
            break;
        case Pin::PC_3:
            *instance = TIM1;
            *channel = TIM_CHANNEL_4;
            break;
        // complementary channel
        case Pin::PC_13:
            *instance = TIM1;
            *channel = TIM_CHANNEL_1;
            break;
        // complementary channel
        case Pin::PF_0:
            *instance = TIM1;
            *channel = TIM_CHANNEL_3;
            break;
        default:
            *instance = NULL;
            *channel = -1;
    }

}

PWMf302x8::PWMf302x8(Pin pin) : PWM(pin) {

    HAL_Init();
    TIM_TypeDef* instance;
    uint16_t channel;
    uint32_t alternateFunction;
    getInstance(pin, &instance, &channel, &alternateFunction);

    systemClockInit();

    if (instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
    else if (instance == TIM15) {
        __HAL_RCC_TIM15_CLK_ENABLE();
    }
    else if (instance == TIM16) {
        __HAL_RCC_TIM16_CLK_ENABLE();
    }
    else if (instance == TIM17) {
        __HAL_RCC_TIM17_CLK_ENABLE();
    }


    TIM_MasterConfigTypeDef masterConfig;
    /*
    // GPIO initialization
    GPIO_InitTypeDef gpioInit;
    gpioInit.Pin = static_cast<uint32_t>(
            1 << static_cast<uint32_t>(pin) & 0xF);
    gpioInit.Mode = GPIO_AF2_TIM1;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;


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
        case 0x3:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            HAL_GPIO_Init(GPIOD, &gpioInit);
            break;
        default:
            break;
    }
    */


     /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  halTIM.Instance = TIM16;
  halTIM.Init.Prescaler = 0;
  halTIM.Init.CounterMode = TIM_COUNTERMODE_UP;
  halTIM.Init.Period = 65535;
  halTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  halTIM.Init.RepetitionCounter = 0;
  halTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&halTIM) != HAL_OK)
  {
  }
  if (HAL_TIM_PWM_Init(&halTIM) != HAL_OK)
  {
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 10000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&halTIM, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&halTIM, &sBreakDeadTimeConfig) != HAL_OK)
  {
  }
 GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(halTIM.Instance==TIM16)
  {
  /* USER CODE BEGIN TIM16_MspPostInit 0 */

  /* USER CODE END TIM16_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM16 GPIO Configuration
    PA6     ------> TIM16_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM16;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM16_MspPostInit 1 */

  /* USER CODE END TIM16_MspPostInit 1 */
  }


  HAL_TIM_PWM_Start(&halTIM, TIM_CHANNEL_1);

    // Initialize the timer for PWM
#if 0
    halTIM.Instance = instance;
    halTIM.Init.Prescaler = 0;
    halTIM.Init.CounterMode = TIM_COUNTERMODE_UP;
    halTIM.Init.Period = 0;
    halTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&halTIM);

    masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&halTIM, &masterConfig);

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&halTIM, &sClockSourceConfig);

    // Channel setup
    halChannel.OCMode = TIM_OCMODE_PWM1;
    halChannel.Pulse = 0;
    halChannel.OCPolarity = TIM_OCPOLARITY_HIGH;
    halChannel.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&halTIM, &halChannel, halTIMChannelID);
#endif
}

void PWMf302x8::setDutyCycle(float dutyCycle) {
    this->dutyCycle = dutyCycle;
}

void PWMf302x8::setPeriod(uint32_t period) {
    this->period = period;

    HAL_TIM_PWM_Stop(&halTIM, halTIMChannelID);

    halTIM.Init.Period = period;
    HAL_TIM_PWM_Init(&halTIM);

    TIM_OC_InitTypeDef channelConfig;
    channelConfig.OCMode = TIM_OCMODE_PWM1;
    channelConfig.Pulse = 10000;
    channelConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    channelConfig.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&halTIM, &channelConfig, halTIMChannelID);

    HAL_TIM_PWM_Start(&halTIM, halTIMChannelID);
}

float PWMf302x8::getDutyCycle() {
    return dutyCycle;
}

uint32_t PWMf302x8::getPeriod() {
    return period;
}

}  // namespace EVT::core::IO
