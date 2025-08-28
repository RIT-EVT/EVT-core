
#include <core/io/platform/f3xx/GPIOf3xx.hpp>
#include <core/io/platform/f3xx/PWM_INPUTf3xx.hpp>

namespace core::io {





PWM_INPUTf3xx::PWM_INPUTf3xx(core::io::Pin pin) {
    TIM_TypeDef* instance;
    uint32_t alternateFunction;
  //getInstance(pin, &instance, &halTIMChannelID, &alternateFunction);

    if (instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    } else if (instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    } else if (instance == TIM15) {
        __HAL_RCC_TIM15_CLK_ENABLE();
    }

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig       = {0};
    TIM_IC_InitTypeDef sConfigIC              = {0};
    TIM_MasterConfigTypeDef sMasterConfig     = {0};


    halTIM.Instance               = instance;
    halTIM.Init.Prescaler         = 0;
    halTIM.Init.CounterMode       = TIM_COUNTERMODE_UP;
    halTIM.Init.Period            = 4294967295;
    halTIM.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    halTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


    HAL_TIM_Base_Init(&halTIM);
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_PWM_Init(&halTIM);
    HAL_TIM_ConfigClockSource(&halTIM, &sClockSourceConfig);
    HAL_TIM_IC_Init(&halTIM);

    sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger     = triggerSource;
    sSlaveConfig.TriggerPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
    sSlaveConfig.TriggerFilter    = 0;

    HAL_TIM_SlaveConfigSynchro(&halTIM, &sSlaveConfig);

    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;

   // HAL_TIM_IC_ConfigChannel(&halTIM, &sConfigIC, directChannel);

    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;

    //HAL_TIM_IC_ConfigChannel(&halTIM, &sConfigIC, indirectChannel);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;

    HAL_TIMEx_MasterConfigSynchronization(&halTIM, &sMasterConfig);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
   Pin myPins[]              = {pin};
    uint8_t numOfPins         = 1;

    GPIO_InitStruct.Pin       = pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = altFunc;
    HAL_GPIO_Init(gpioType, &GPIO_InitStruct);
}



uint32_t PWM_INPUTf3xx::getDutyCycle() {
    return dutyCycle;
}

uint32_t PWM_INPUTf3xx::getPeriod() {
    return period;
}

uint32_t PWM_INPUTf3xx::getFrequency() {
    return frequency;
}







}
