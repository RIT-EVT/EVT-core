/**
 * @brief This file contains all the types used by timers in EVT-Core.
 *
 * @details
 * This file contains type definitions for the timer configuration structure, and support enums.
 *
 * TimerConfiguration_t is a configuration structure that timers use to set themselves up.
 *
 * The enums TimerCounterMode, TimerClockDivision, TimerAutoReloadPreload, TimerClockSource,
 * TimerMasterModeSelection, and TimerMasterSlaveMode, are user facing declarations of HAL definitions
 * used by TimerConfiguration_t structs.
 *
 * This file has one oddity, the HAL for supported boards (F3 & fF4) is included to give access to definitions used
 * in the enums. Importing these into this file was chosen, instead of two separate F3 & F4 files, because there are
 * no differences in the definition names, or supported values between the boards. This file is also NOT included
 * into Timer.hpp to reduce the footprint of HAL includes.
 */

#ifndef EVT_TIMERTYPES_HPP
#define EVT_TIMERTYPES_HPP

#ifdef STM32F3xx
    #include <HALf3/stm32f3xx.h>
#endif

#ifdef STM32F4xx
    #include <HALf4/stm32f4xx.h>
#endif

namespace core::dev {
enum class TimerCounterMode {
    /** Counter used as up-counter   */
    UP = TIM_COUNTERMODE_UP,
    /** Counter used as down-counter */
    DOWN = TIM_COUNTERMODE_DOWN,
    /** Center-aligned mode 1 */
    CENTRAL_ALIGNED_1 = TIM_COUNTERMODE_CENTERALIGNED1,
    /** Center-aligned mode 2 */
    CENTRAL_ALIGNED_2 = TIM_COUNTERMODE_CENTERALIGNED2,
    /** Center-aligned mode 3 */
    CENTRAL_ALIGNED_3 = TIM_COUNTERMODE_CENTERALIGNED3,
};

enum class TimerClockDivision {
    /** Clock division: tDTS=tCK_INT */
    DIVISION_1 = TIM_CLOCKDIVISION_DIV1,
    /** Clock division: tDTS=2*tCK_INT */
    DIVISION_2 = TIM_CLOCKDIVISION_DIV2,
    /** Clock division: tDTS=4*tCK_INT */
    DIVISION_4 = TIM_CLOCKDIVISION_DIV4,
};

enum class TimerAutoReloadPreload {
    /** TIMx_ARR register is not buffered */
    DISABLE = TIM_AUTORELOAD_PRELOAD_DISABLE,
    /** < TIMx_ARR register is buffered */
    ENABLE = TIM_AUTORELOAD_PRELOAD_ENABLE,
};

enum class TimerClockSource {
    EXTERNAL_MODE_2 = TIM_CLOCKSOURCE_ETRMODE2,
    INTERNAL = TIM_CLOCKSOURCE_INTERNAL,
    EXTERNAL_MODE_1_ITR0 = TIM_CLOCKSOURCE_ITR0,
    EXTERNAL_MODE_1_ITR1 = TIM_CLOCKSOURCE_ITR1,
    EXTERNAL_MODE_1_ITR2 = TIM_CLOCKSOURCE_ITR2,
    EXTERNAL_MODE_1_ITR3 = TIM_CLOCKSOURCE_ITR3,
    EXTERNAL_MODE_1_TTI1FP1_ED = TIM_CLOCKSOURCE_TI1ED,
    EXTERNAL_MODE_1_TTI1FP1 = TIM_CLOCKSOURCE_TI1,
    EXTERNAL_MODE_1_TTI2FP2 = TIM_CLOCKSOURCE_TI2,
    EXTERNAL_MODE_1_ETRF = TIM_CLOCKSOURCE_ETRMODE1
};

enum class TimerMasterModeSelection {
    /*!< TIMx_EGR.UG bit is used as trigger output (TRGO)              */
    RESET = TIM_TRGO_RESET,
    /*!< TIMx_CR1.CEN bit is used as trigger output (TRGO)             */
    ENABLE = TIM_TRGO_ENABLE,
    /*!< Update event is used as trigger output (TRGO)                 */
    UPDATE = TIM_TRGO_UPDATE,
    /*!< Capture or a compare match 1 is used as trigger output (TRGO) */
    OC1 = TIM_TRGO_OC1,
    /*!< OC1REF signal is used as trigger output (TRGO)                */
    OC1REF = TIM_TRGO_OC1REF,
    /*!< OC2REF signal is used as trigger output (TRGO)                */
    OC2REF = TIM_TRGO_OC2REF,
    /*!< OC3REF signal is used as trigger output(TRGO)                 */
    OC3REF = TIM_TRGO_OC3REF,
    /*!< OC4REF signal is used as trigger output(TRGO)                 */
    OC4REF = TIM_TRGO_OC4REF,
};

enum class TimerMasterSlaveMode {
    /*!< No action */
    ENABLE = TIM_MASTERSLAVEMODE_ENABLE,
    /*!< Master/slave mode is selected */
    DISABLE = TIM_MASTERSLAVEMODE_DISABLE,
};

/**
 * @brief A timer configuration provides a timer with all configuration variables needed to be set up..
 */
typedef struct {
    /**
     * Specifies how the timer will count.
     * Maps to a timer's @ref TIM_Base_InitTypeDef:CounterMode
     */
    TimerCounterMode counterMode{};

    /**
     * Specifies the clock division.
     * Maps to a timer's @ref TIM_Base_InitTypeDef:ClockDivision
     */
    TimerClockDivision clockDivision{};

    /**
     * Specifies the auto reload preload value, which controls whether @ref TIMx_ARR is buffered or not.
     * Maps to a timer's @ref TIM_Base_InitTypeDef:AutoReloadPreload
     */
    TimerAutoReloadPreload autoReloadPreload{};

    /**
     * Timer clock source
     * Maps to @ref TIM_ClockConfigTypeDef:ClockSource
     */
    TimerClockSource clockSource{};

    /**
     * Trigger output (TRGO) selection.
     * Maps to @ref TIM_MasterConfigTypeDef:MasterOutputTrigger
     */
    TimerMasterModeSelection masterOutputTrigger{};

    /**
     * Master/slave mode selection.
     * This parameter can be a value of @ref TIM_Master_Slave_Mode
     * Maps to @ref TIM_MasterConfigTypeDef:MasterSlaveMode
     */
    TimerMasterSlaveMode masterSlaveMode{};
} TimerConfiguration_t;
} // namespace core::dev
#endif // EVT_TIMERTYPES_HPP
