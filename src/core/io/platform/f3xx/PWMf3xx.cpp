#include <core/io/pin.hpp>
#include <core/io/platform/f3xx/GPIOf3xx.hpp>
#include <core/io/platform/f3xx/PWMf3xx.hpp>
#include <core/utils/log.hpp>

namespace core::io {

/**
 * Get the TIM instance and channel associated with a pin. This information
 * is pulled from the STM32F302x8 documentation. Easier representation of this
 * data can be found on MBed's STM32F302r8 documentation.
 *
 * @param[in] pin The pin to check the instance of
 * @param[in] channel The channel to assign to
 * @param[in] alternateFunction The GPIO identifier for the function of the pin
 */
static void getInstance(Pin pin, uint32_t* channel, uint32_t* alternateFunction) {
    switch (pin) {
#if defined(STM32F302x8)
    case Pin::PA_0:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    ////////////////////////////////////////////
    case Pin::PA_1:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF9_TIM15;
        break;
    case Pin::PA_2:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF9_TIM15;
        break;
    case Pin::PA_3:
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
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM2;
        break;
    case Pin::PA_8:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_6:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_7:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PA_9:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_10:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_11:
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF11_TIM1;
        break;
    case Pin::PA_12:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PA_13:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_15:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM2;
        break;
    // complementary channel
    case Pin::PB_0:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    // complementary channel
    case Pin::PB_1:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_3:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM2;
    case Pin::PB_4:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_5:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM17;
        break;
    // complementary channel
    case Pin::PB_6:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PB_7:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_8:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_9:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_10:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PB_11:
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF1_TIM2;
        break;

    // complementation channel
    case Pin::PB_13:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_14:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PB_15:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PC_0:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_1:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_2:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_3:
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    // complementary channel
    case Pin::PC_13:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF4_TIM1;
        break;
    // complementary channel
    case Pin::PF_0:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
#elif defined(STM32F334x8)
    case Pin::PA_0:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    case Pin::PA_1:
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
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_6:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_7:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PA_9:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_10:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PA_11:
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF11_TIM1;
        break;
    case Pin::PA_12:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PA_13:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PA_15:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    case Pin::PB_0:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    // complementary channel
    case Pin::PB_1:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_3:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PB_4:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_5:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF10_TIM17;
        break;
    // complementary channel
    case Pin::PB_6:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    // complementary channel
    case Pin::PB_7:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_8:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM16;
        break;
    case Pin::PB_9:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM17;
        break;
    case Pin::PB_10:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    case Pin::PB_11:
        *channel           = TIM_CHANNEL_4;
        *alternateFunction = GPIO_AF1_TIM2;
        break;
    // complementary channel
    case Pin::PB_13:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
    case Pin::PB_14:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PB_15:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF1_TIM15;
        break;
    case Pin::PC_0:
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_1:
        *channel           = TIM_CHANNEL_2;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_2:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF2_TIM1;
        break;
    case Pin::PC_3:
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
        *channel           = TIM_CHANNEL_1;
        *alternateFunction = GPIO_AF4_TIM1;
        break;
    // complementary channel
    case Pin::PF_0:
        *channel           = TIM_CHANNEL_3;
        *alternateFunction = GPIO_AF6_TIM1;
        break;
#endif
    default:
        *channel           = -1;
        *alternateFunction = -1;
    }
}

PWMf3xx::PWMf3xx(Pin pin, TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, dev::TimerConfiguration configuration,
                 uint32_t clockPrescaler)
    : PWM(pin), TimerF3xx(timerPeripheral, clockPeriod, configuration, clockPrescaler) {
    uint32_t alternateFunction;
    getInstance(pin, &halTIMChannelID, &alternateFunction);

    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
    HAL_TIM_PWM_Init(halTimer);

    sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime         = 0;
    sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter      = 0;
    sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
    HAL_TIMEx_ConfigBreakDeadTime(halTimer, &sBreakDeadTimeConfig);

    // Setup GPIO pin for PMW
    GPIO_InitTypeDef gpioInit = {0};
    Pin myPins[]              = {pin};
    uint8_t numOfPins         = 1;

    GPIOf3xx::gpioStateInit(
        &gpioInit, myPins, numOfPins, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, alternateFunction);

    TimerF3xx::startTimer();
}

void PWMf3xx::setDutyCycle(uint32_t dutyCycle) {
    this->dutyCycle = dutyCycle;

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode             = TIM_OCMODE_PWM1;
    sConfigOC.Pulse              = dutyCycle * halTimer->Init.Period / 100;
    sConfigOC.OCPolarity         = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity        = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode         = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState        = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState       = TIM_OCNIDLESTATE_RESET;

    HAL_TIM_PWM_Stop(halTimer, halTIMChannelID);
    HAL_TIM_PWM_ConfigChannel(halTimer, &sConfigOC, halTIMChannelID);
    HAL_TIM_PWM_Start(halTimer, halTIMChannelID);
}

void PWMf3xx::setPeriod(uint32_t period) {
    if (period == 0) {
        period = 1;
    }

    this->period = period;
    HAL_TIM_PWM_Stop(halTimer, halTIMChannelID);

    uint32_t autoReload;
    uint32_t prescaler      = -1;
    uint64_t clockFrequency = HAL_RCC_GetHCLKFreq();

    // Required loop in order to determine a prescaler which will bring the
    // autoreload value into a valid range.
    do {
        prescaler++;
        autoReload = period * clockFrequency / (prescaler + 1) / 1000000 - 1;
    } while (autoReload > 65535);

    TimerF3xx::setPeriod(autoReload, prescaler);
    TimerF3xx::startTimer();

    HAL_TIM_PWM_Start(halTimer, halTIMChannelID);

    // Duty cycle value depends on period, need to update duty cycle
    setDutyCycle(this->dutyCycle);
}

uint32_t PWMf3xx::getDutyCycle() {
    return this->dutyCycle;
}

uint32_t PWMf3xx::getPeriod() {
    return this->period;
}
} // namespace core::io
