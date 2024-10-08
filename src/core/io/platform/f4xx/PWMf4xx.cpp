#include <core/io/platform/f4xx/GPIOf4xx.hpp>
#include <core/io/platform/f4xx/PWMf4xx.hpp>

namespace core::io {

/**
 * Get the TIM instance and channel associated with a pin. This information
 * is pulled from the STM32F446RE documentation.
 *
 * @param pin The pin to check the instance of
 * @param instance The instance to assign to
 * @param channel The channel to assign to
 * @param alternateFunction The GPIO identifier for the function of the pin
 */
static void getInstance(Pin pin, TIM_TypeDef** instance, uint32_t* channel, uint32_t* alternateFunction) {
    switch (pin) {
        // TIMER 1
    case Pin::PA_8:
        *instance          = TIM1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM1;
        break;
    case Pin::PA_9:
        *instance          = TIM1;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM1;
        break;
    case Pin::PA_10:
        *instance          = TIM1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF1_TIM1;
        break;
    case Pin::PA_11:
        *instance          = TIM1;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF1_TIM1;
        break;

        // TIMER 2
    case Pin::PA_0:
        *instance          = TIM2;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PA_1:
        *instance          = TIM2;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PA_2:
        *instance          = TIM2;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PA_3:
        *instance          = TIM2;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF1_TIM2;
        break;

        // TIMER 3
    case Pin::PC_6:
        *instance          = TIM3;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF2_TIM3;
        break;
    case Pin::PC_7:
        *instance          = TIM3;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF2_TIM3;
        break;
    case Pin::PC_8:
        *instance          = TIM3;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF2_TIM3;
        break;
    case Pin::PC_9:
        *instance          = TIM3;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF2_TIM3;
        break;

        // TIMER 4
    case Pin::PB_6:
        *instance          = TIM4;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF2_TIM4;
        break;
    case Pin::PB_7:
        *instance          = TIM4;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF2_TIM4;
        break;
    case Pin::PB_8:
        *instance          = TIM4;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF2_TIM4;
        break;
    case Pin::PB_9:
        *instance          = TIM4;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF2_TIM4;
        break;
    default:
        *instance          = NULL;
        *channel           = -1;
        *alternateFunction = -1;
    }
}

PWMf4xx::PWMf4xx(Pin pin) : PWM(pin) {
    TIM_TypeDef* instance;
    uint32_t alternateFunction;
    getInstance(pin, &instance, &halTIMChannelID, &alternateFunction);

    if (instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    } else if (instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    } else if (instance == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    } else if (instance == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE();
    }

    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    halTIM.Instance               = instance;
    halTIM.Init.Prescaler         = 0;
    halTIM.Init.CounterMode       = TIM_COUNTERMODE_UP;
    halTIM.Init.Period            = 0;
    halTIM.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    halTIM.Init.RepetitionCounter = 0;
    halTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&halTIM);

    HAL_TIM_PWM_Init(&halTIM);

    sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime         = 0;
    sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter      = 0;
    sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
    HAL_TIMEx_ConfigBreakDeadTime(&halTIM, &sBreakDeadTimeConfig);

    // Setup GPIO pin for PMW
    GPIO_InitTypeDef gpioInit = {0};
    Pin myPins[]              = {pin};
    uint8_t numOfPins         = 1;

    GPIOf4xx::gpioStateInit(
        &gpioInit, myPins, numOfPins, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, alternateFunction);
}

void PWMf4xx::setDutyCycle(uint32_t dutyCycle) {
    this->dutyCycle = dutyCycle;

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode             = TIM_OCMODE_PWM1;
    sConfigOC.Pulse              = dutyCycle * halTIM.Init.Period / 100;
    sConfigOC.OCPolarity         = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity        = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode         = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState        = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState       = TIM_OCNIDLESTATE_RESET;

    HAL_TIM_PWM_Stop(&halTIM, halTIMChannelID);
    HAL_TIM_PWM_ConfigChannel(&halTIM, &sConfigOC, halTIMChannelID);
    HAL_TIM_PWM_Start(&halTIM, halTIMChannelID);
}

void PWMf4xx::setPeriod(uint32_t period) {
    if (period == 0) {
        period = 1;
    }

    this->period = period;
    HAL_TIM_PWM_Stop(&halTIM, halTIMChannelID);

    uint32_t autoReload;
    uint32_t prescaler      = -1;
    uint64_t clockFrequency = HAL_RCC_GetSysClockFreq();

    // Required loop in order to determine a prescaler which will bring the
    // autoreload value into a valid range.
    do {
        prescaler++;
        autoReload = period * clockFrequency / (prescaler + 1) / 1000000 - 1;
    } while (autoReload > 65535);

    halTIM.Init.Period    = autoReload;
    halTIM.Init.Prescaler = prescaler;
    HAL_TIM_Base_Init(&halTIM);
    HAL_TIM_PWM_Start(&halTIM, halTIMChannelID);

    // Duty cycle value depends on period, need to update duty cycle
    setDutyCycle(this->dutyCycle);
}

uint32_t PWMf4xx::getDutyCycle() {
    return dutyCycle;
}

uint32_t PWMf4xx::getPeriod() {
    return period;
}

} // namespace core::io
