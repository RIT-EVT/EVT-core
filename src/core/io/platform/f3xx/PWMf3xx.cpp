#include <core/io/platform/f3xx/GPIOf3xx.hpp>
#include <core/io/platform/f3xx/PWMf3xx.hpp>
#include <core/manager.hpp>

namespace core::io {

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
static void getInstance(Pin pin, dev::MCUTimer* instance, uint32_t* channel, uint32_t* alternateFunction) {
    switch (pin) {
#if defined(STM32F302x8)
    case Pin::PA_0:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    ////////////////////////////////////////////
    case Pin::PA_1:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF9_TIM15;
        break;
    case Pin::PA_2:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF9_TIM15;
        break;
    case Pin::PA_3:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF3_TIM15;
        break;
        /////////////////////////////////////////////////////////////
        //    case Pin:: PA_4:
        //        *instance = TIM3;
        //        *channel = TIM_CHANNEL_2;
        //        *alternateFunction = GPIO_AF2_TIM3;
        //        break;
    case Pin::PA_5:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM2;
        break;
    case Pin::PA_8:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_6:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_7:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PA_9:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_10:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_11:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF11_TIM1;
        break;
    case Pin::PA_12:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PA_13:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_15:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM2;
        break;
    // complementary channel
    case Pin::PB_0:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    // complementary channel
    case Pin::PB_1:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_3:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM2;
    case Pin::PB_4:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_5:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM17;
        break;
    // complementary channel
    case Pin::PB_6:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PB_7:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_8:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_9:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_10:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PB_11:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF1_TIM2;
        break;

    // complementation channel
    case Pin::PB_13:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_14:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PB_15:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PC_0:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_1:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_2:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_3:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    // complementary channel
    case Pin::PC_13:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF4_TIM1;
        break;
    // complementary channel
    case Pin::PF_0:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
#elif defined(STM32F334x8)
    case Pin::PA_0:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    case Pin::PA_1:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF2_TIM15;
        break;
        // PWM not shown for PA_2 - PA_5
        //     case Pin::PA_2:
        //     *instance = TIM15;
        //     *channel = TIM_CHANNEL_1;
        //     *alternateFunction = GPIO_AF1_TIM15;
        //     break;
        //     case Pin::PA_3:
        //     *instance = TIM15;
        //     *channel = TIM_CHANNEL_2;
        //     *alternateFunction = GPIO_AF2_TIM15;
        //     break;
        //     case Pin:: PA_4:
        //     *instance = TIM3;
        //     *channel = TIM_CHANNEL_2;
        //     *alternateFunction = GPIO_AF2_TIM3;
        //     break;
        //     case Pin::PA_5:
        //     *instance = TIM2;
        //     *channel = TIM_CHANNEL_1;
        //     *alternateFunction = GPIO_AF10_TIM2;
        //     break;
    case Pin::PA_8:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_6:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_7:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PA_9:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_10:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_11:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF11_TIM1;
        break;
    case Pin::PA_12:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PA_13:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_15:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    case Pin::PB_0:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    // complementary channel
    case Pin::PB_1:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_3:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PB_4:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_5:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM17;
        break;
    // complementary channel
    case Pin::PB_6:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PB_7:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_8:
        *instance          = dev::MCUTimer::Timer16;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_9:
        *instance          = dev::MCUTimer::Timer17;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_10:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PB_11:
        *instance          = dev::MCUTimer::Timer2;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    case Pin::PB_13:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_14:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PB_15:
        *instance          = dev::MCUTimer::Timer15;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PC_0:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_1:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_2:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_3:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
        // PWM not shown for PC_6 - PC_9
        //     case Pin::PC_6:
        //     *instance = TIM3;
        //     *channel = TIM_CHANNEL_1;
        //     *alternateFunction = GPIO_AF2_TIM3;
        //     break;
        //     case Pin::PC_7 :
        //     *instance = TIM3;
        //     *channel = TIM_CHANNEL_1;
        //     *alternateFunction = GPIO_AF10_TIM3;
        //     break;
        //     case Pin::PC_8:
        //     *instance = TIM3;
        //     *channel = TIM_CHANNEL_3;
        //     *alternateFunction = GPIO_AF2_TIM3;
        //     break;
        //     case Pin::PC_9:
        //     *instance = TIM3;
        //     *channel = TIM_CHANNEL_4;
        //     *alternateFunction = GPIO_AF10_TIM3;
        //     break;
        //  complementary channel
    case Pin::PC_13:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF4_TIM1;
        break;
    // complementary channel
    case Pin::PF_0:
        *instance          = dev::MCUTimer::Timer1;
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
#endif
    default:
        *instance          = dev::MCUTimer::None;
        *channel           = -1;
        *alternateFunction = -1;
    }
}

static void setTimer(dev::MCUTimer instance, dev::Timer* timer) {
    dev::TimerConfiguration timerConfiguration = {
        TIM_COUNTERMODE_UP,
        TIM_CLOCKDIVISION_DIV1,
        TIM_AUTORELOAD_PRELOAD_ENABLE
        // May need to finish filling this out 🤷‍♀️
    };

    switch (instance) {
        #if defined(STM32F302x8)
    case dev::MCUTimer::Timer1:
        *timer = dev::getTimer<dev::MCUTimer::Timer1>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer2:
        *timer = dev::getTimer<dev::MCUTimer::Timer2>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer6:
        *timer = dev::getTimer<dev::MCUTimer::Timer6>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer15:
        *timer = dev::getTimer<dev::MCUTimer::Timer15>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer16:
        *timer = dev::getTimer<dev::MCUTimer::Timer16>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer17:
        *timer = dev::getTimer<dev::MCUTimer::Timer17>(0, timerConfiguration);
        break;
        #elif defined(STM32F334x8)
    case dev::MCUTimer::Timer1:
        *timer = dev::getTimer<dev::MCUTimer::Timer11>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer2:
        *timer = dev::getTimer<dev::MCUTimer::Timer12>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer3:
        *timer = dev::getTimer<dev::MCUTimer::Timer3>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer6:
        *timer = dev::getTimer<dev::MCUTimer::Timer6>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer7:
        *timer = dev::getTimer<dev::MCUTimer::Timer7>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer15:
        *timer = dev::getTimer<dev::MCUTimer::Timer15>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer16:
        *timer = dev::getTimer<dev::MCUTimer::Timer16>(0, timerConfiguration);
        break;
    case dev::MCUTimer::Timer17:
        *timer = dev::getTimer<dev::MCUTimer::Timer17>(0, timerConfiguration);
        break;
        #endif
    case dev::MCUTimer::None:
        break;
        }
}

PWMf3xx::PWMf3xx(Pin pin) : PWM(pin) {
    dev::MCUTimer instance;
    uint32_t alternateFunction;
    getInstance(pin, &instance, &halTIMChannelID, &alternateFunction);

    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
    setTimer(instance, &timer);

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

    GPIOf3xx::gpioStateInit(
        &gpioInit, myPins, numOfPins, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, alternateFunction);
}

void PWMf3xx::setDutyCycle(uint32_t dutyCycle) {
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

void PWMf3xx::setPeriod(uint32_t period) {
    if (period == 0) {
        period = 1;
    }

    this->period = period;
    HAL_TIM_PWM_Stop(&halTIM, halTIMChannelID);

    uint32_t autoReload;
    uint32_t prescaler      = -1;
    uint64_t clockFrequency = HAL_RCC_GetHCLKFreq();

    // Required loop in order to determine a prescaler which will bring the
    // autoreload value into a valid range.
    do {
        prescaler++;
        autoReload = period * clockFrequency / (prescaler + 1) / 1000000 - 1;
    } while (autoReload > 65535);

    timer.setPeriod(autoReload, period);
    HAL_TIM_Base_Init(&halTIM);
    HAL_TIM_PWM_Start(&halTIM, halTIMChannelID);

    // Duty cycle value depends on period, need to update duty cycle
    setDutyCycle(this->dutyCycle);
}

uint32_t PWMf3xx::getDutyCycle() {
    return dutyCycle;
}

uint32_t PWMf3xx::getPeriod() {
    return period;
}

} // namespace core::io
