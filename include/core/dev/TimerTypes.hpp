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
/**
 * @brief Timer Counter Mode, analogous to TIM_Counter_Mode.
 * @details
 * Mirrors the definitions that are grouped under TIM_Counter_Mode. Adds extra documentation & easier use.
 */
enum class TimerCounterMode {
    /** The timer's counter, counts up from 0 to the period */
    UP = TIM_COUNTERMODE_UP,
    /** The timer's counter, counts down from the period to 0. */
    DOWN = TIM_COUNTERMODE_DOWN,
    /**
     * @brief Center-aligned mode 1, used for PWM.
     * @details
     * Behavior: The interrupt flags for triggers (Update & Capture/Compare) are only set when the counter is counting **down**.
     * Compare Event: When the counter matches the CCR value while counting up.
     */
    CENTRAL_ALIGNED_1 = TIM_COUNTERMODE_CENTERALIGNED1,
    /**
     * @brief Center-aligned mode 2, used for PWM.
     * @details
     * Behavior: The interrupt flags for triggers (Update & Capture/Compare) are only set when the counter is counting **up**.
     * Compare Event: When the counter matches the CCR value while counting down.
     */
    CENTRAL_ALIGNED_2 = TIM_COUNTERMODE_CENTERALIGNED2,
    /**
     * @brief Center-aligned mode 3, used for PWM.
     * @details
     * Behavior: The interrupt flags for triggers (Update & Capture/Compare) are set when the counter is counting **up** and counting **down**
     * Compare Event: When the counter matches the CCR value while counting up and down. The event will occur twice per PWM period.
     */
    CENTRAL_ALIGNED_3 = TIM_COUNTERMODE_CENTERALIGNED3,
};

/**
 * @brief Timer Clock Division, indicates the clock division ratio.
 * @details
 * The elements of this enum are used in the clock division bit field. They indicate the division ratio between
 * the timer clock (CK_INT) frequency and the dead-time. The dead-time is used by the sample clock (tDTS),
 * dead-time generators, and the digital filters (ETR, TIx)
 */
enum class TimerClockDivision {
    /**
     * @brief Clock division: tDTS=tCK_INT
     * @details
     * The sample clock frequency (tDTS) = Timer clock (CK_INT) frequency
     */
    DIVISION_1 = TIM_CLOCKDIVISION_DIV1,
    /**
     * @brief Clock division: tDTS=2*tCK_INT
     * @details
     * The sample clock frequency (tDTS) = 2 * Timer clock (CK_INT) frequency
     * 2x `DIVISION_1`
    */
    DIVISION_2 = TIM_CLOCKDIVISION_DIV2,
    /**
     * @brief Clock division: tDTS=4*tCK_INT
     * @details
     * The sample clock frequency (tDTS) = 4 * Timer clock (CK_INT) frequency
     * 4x `DIVISION_1`
    */
    DIVISION_4 = TIM_CLOCKDIVISION_DIV4,
};

/**
 * @brief Indicates whether the Auto Preload Reload register is buffed
 */
enum class TimerAutoReloadPreload {
    /** Timer auto reload preload register is not buffered */
    NO_BUFFER = TIM_AUTORELOAD_PRELOAD_DISABLE,
    /** Timer auto reload preload register is buffered */
    BUFFER = TIM_AUTORELOAD_PRELOAD_ENABLE,
};

/**
 * @brief The source for clock signals that the timer will use.
 * @details
 * 
 */
enum class TimerClockSource {
    /**
     * @brief External Clock source mode 2
     * @details
     * When external clock source mode 2 is enabled, the timer counter is clocked whenever there is an active
     * edge on the ETRF signal.
     */
    EXTERNAL_MODE_2 = TIM_CLOCKSOURCE_ETRMODE2,
    /**
     * @brief Internal clock source
     * @details
     *
     */
    INTERNAL = TIM_CLOCKSOURCE_INTERNAL,
    /**
     * @brief External Clock Source, Mode 1 (ITR0)
     * @details
     *
     */
    EXTERNAL_MODE_1_ITR0 = TIM_CLOCKSOURCE_ITR0,
    /**
     * @brief External Clock Source, Mode 1 (ITR1)
     * @details
     *
     */
    EXTERNAL_MODE_1_ITR1 = TIM_CLOCKSOURCE_ITR1,
    /**
    * @brief External Clock Source, Mode 1 (ITR2)
    * @details
    *
    */
    EXTERNAL_MODE_1_ITR2 = TIM_CLOCKSOURCE_ITR2,
    /**
     * @brief External Clock Source, Mode 1 (ITR3)
     * @details
     *
     */
    EXTERNAL_MODE_1_ITR3 = TIM_CLOCKSOURCE_ITR3,
    /**
     * @brief External Clock Source, Mode 1 (TTI1FP1 + edge detect)
     * @details
     *
     */
    EXTERNAL_MODE_1_TTI1FP1_ED = TIM_CLOCKSOURCE_TI1ED,
    /**
     * @brief External Clock Source, Mode 1 (TTI1FP1)
     * @details
     *
     */
    EXTERNAL_MODE_1_TTI1FP1 = TIM_CLOCKSOURCE_TI1,
    /**
     * @brief External Clock Source, Mode 1 (TTI2FP2)
     * @details
     *
     */
    EXTERNAL_MODE_1_TTI2FP2 = TIM_CLOCKSOURCE_TI2,
    /**
     * @brief External Clock Source, Mode 1 (ETRF)
     * @details
     *
     */
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
