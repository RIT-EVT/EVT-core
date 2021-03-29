#include <EVT/io/platform/f3xx/f302x8/PWMf302x8.hpp>

namespace EVT::core::IO {

/**
 * Get the TIM instance and channel associated with a pin. This information
 * is pulled from the STM32F302x8 documentation. Easier representation of this
 * data can be found on MBed's STM32F302r8 documentation.
 *
 * @param pin The pin to check the instance of
 * @param instance The instance to assign to
 * @param channel The channel to assign to
 */
static void getInstance(Pin pin, TIM_TypeDef** instance, uint16_t* channel) {
    switch (pin) {
        // complementary channel
        case Pin::PA_1:
            *instance = TIM15;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PA_2:
            *instance = TIM15;
            *channel = TIM_CHANNEL_1;
            break;
        case Pin::PA_3:
            *instance = TIM15;
            *channel = TIM_CHANNEL_2;
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

    TIM_TypeDef* instance;
    uint16_t channel;
    getInstance(pin, &instance, &channel);

    TIM_MasterConfigTypeDef masterConfig;

    // Initialize the timer for PWM
    halTIM.Instance = instance;
    halTIM.Init.Prescaler = 0;
    halTIM.Init.CounterMode = TIM_COUNTERMODE_UP;
    halTIM.Init.Period = 0;
    halTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&halTIM);

    masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&halTIM, &masterConfig);

    // Channel setup
    halChannel.OCMode = TIM_OCMODE_PWM1;
    halChannel.Pulse = 0;
    halChannel.OCPolarity = TIM_OCPOLARITY_HIGH;
    halChannel.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&halTIM, &halChannel, halTIMChannelID);
}

void PWMf302x8::setDutyCycle(float dutyCycle) {
    this->dutyCycle = dutyCycle;
}

void PWMf302x8::setPeriod(uint32_t period) {
    this->period = period;
}

float PWMf302x8::getDutyCycle() {
    return dutyCycle;
}

uint32_t PWMf302x8::getPeriod() {
    return period;
}

}  // namespace EVT::core::IO
