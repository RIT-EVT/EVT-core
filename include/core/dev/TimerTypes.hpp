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
     * Compare Event: When the counter matches the CCR value while counting down.
     */
    CENTRAL_ALIGNED_1 = TIM_COUNTERMODE_CENTERALIGNED1,
    /**
     * @brief Center-aligned mode 2, used for PWM.
     * @details
     * Behavior: The interrupt flags for triggers (Update & Capture/Compare) are only set when the counter is counting **up**.
     * Compare Event: When the counter matches the CCR value while counting up.
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
 * @brief Tells the timer what source will initiate a clock.
 * @details
 * Clock sources define when a timer will receive a clock. This can be either an internal or external clock source.
 * External clock sources have many options for configuring when an external signal will actually tigger a clock.
 * Note: Many of these cases need more research. Particularly the external sources, as online information is limited.
 * If you need to use one of these, fallback to the reference document for the MCU you are using (Please update this with more info)
 */
enum class TimerClockSource {
    /**
     * @brief Internal clock source
     * @details
     * The standard setting for timers. This will trigger the timer clock based on the STM32's internal clock.
     */
    INTERNAL = TIM_CLOCKSOURCE_INTERNAL,
    /**
     * @brief External Clock Source, Mode 1 (ITR0)
     * @details
     * The timer's clock source is synchronized to Interrupt 0. This can be used to synchronize multiple timers together.
     * Please see the Timer synchronization section of the Microcontroller Reference Document for more information.
     */
    EXTERNAL_MODE_1_ITR0 = TIM_CLOCKSOURCE_ITR0,
    /**
     * @brief External Clock Source, Mode 1 (ITR1)
     * @details
     * The timer's clock source is synchronized to Interrupt 1. This can be used to synchronize multiple timers together.
     * Please see the Timer synchronization section of the Microcontroller Reference Document for more information.
     */
    EXTERNAL_MODE_1_ITR1 = TIM_CLOCKSOURCE_ITR1,
    /**
    * @brief External Clock Source, Mode 1 (ITR2)
    * @details
    * The timer's clock source is synchronized to Interrupt 2. This can be used to synchronize multiple timers together.
    * Please see the Timer synchronization section of the Microcontroller Reference Document for more information.
    */
    EXTERNAL_MODE_1_ITR2 = TIM_CLOCKSOURCE_ITR2,
    /**
     * @brief External Clock Source, Mode 1 (ITR3)
     * @details
     * The timer's clock source is synchronized to Interrupt 3. This can be used to synchronize multiple timers together.
     * Please see the Timer synchronization section of the Microcontroller Reference Document for more information.
     */
    EXTERNAL_MODE_1_ITR3 = TIM_CLOCKSOURCE_ITR3,
    /**
     * @brief External Clock Source, Mode 1 (TTI1FP1 + edge detect)
     * @details
     * The clock source comes from Filtered Timer Input 1. This should be used if the timer needs to be triggered
     * on the rising or falling edge of a signal.
     */
    EXTERNAL_MODE_1_TTI1FP1_ED = TIM_CLOCKSOURCE_TI1ED,
    /**
     * @brief External Clock Source, Mode 1 (TTI1FP1)
     * @details
     * The clock source comes from Filtered Timer Input 1.
     * Note: More information needed
     */
    EXTERNAL_MODE_1_TTI1FP1 = TIM_CLOCKSOURCE_TI1,
    /**
     * @brief External Clock Source, Mode 1 (TTI2FP2)
     * @details
     * The clock source comes from Filtered Timer Input 2.
     * Note: More information needed
     */
    EXTERNAL_MODE_1_TTI2FP2 = TIM_CLOCKSOURCE_TI2,
    /**
     * @brief External Clock Source, Mode 1 (ETRF)
     * @details
     * The clock source comes from the ETRF trigger / signal.
     * Note: More information needed
     */
    EXTERNAL_MODE_1_ETRF = TIM_CLOCKSOURCE_ETRMODE1,
    /**
     * @brief External Clock source mode 2
     * @details
     * The timer receives a clock whenever there is an active edge on the ETRF trigger / signal.
     * Note: More information needed
     */
    EXTERNAL_MODE_2 = TIM_CLOCKSOURCE_ETRMODE2
};

/**
 * @brief Controls what signals a timer outputs to timers configured as slaves.
 * @details
 * Defines how the Master in a master / slave timer relationship should function. Master mode timers can reset, start,
 * stop, or clock the counter of another timer configured in Slave Mode.
 * Note: Trigger outputs are only affected when TimerMasterSlaveMode is set to ENABLE.
 */
enum class TimerMasterModeSelection {
    /**
     * @brief Sends a RESET signal to slave timers.
     * @details
     * The TIMx_EGR.UG bit is used as the trigger output (TRGO), which sends the RESET signal to
     * any configured slave timers.
     */
    RESET = TIM_TRGO_RESET,
    /**
     * @brief Sends an ENABLE signal to slave timers.
     * @details
     * The TIMx_CR1.CEN bit is used as the trigger output (TRGO), which sends the ENABLE signal to
     * any configured slave timers.
     */
    ENABLE = TIM_TRGO_ENABLE,
    /**
     * @breif Sends an Update event to slave timers.
     */
    UPDATE = TIM_TRGO_UPDATE,
    /**
     * @brief Capture or a compare match 1 is used as trigger output (TRGO)
     */
    OC1 = TIM_TRGO_OC1,
    /**
     * @brief OC1REF signal is used as trigger output (TRGO)
     */
    OC1REF = TIM_TRGO_OC1REF,
    /**
     * @brief OC2REF signal is used as trigger output (TRGO)
     */
    OC2REF = TIM_TRGO_OC2REF,
    /**
     * @brief OC3REF signal is used as trigger output (TRGO)
     */
    OC3REF = TIM_TRGO_OC3REF,
    /**
     * @brief C4REF signal is used as trigger output (TRGO)
     */
    OC4REF = TIM_TRGO_OC4REF,
};

/**
 * @brief Allows a timer to control another set of timers.
 * @details
 * When one timer is configured in Master Mode, it can reset, start, stop, or clock the counter of another timer
 * configured in Slave Mode. Timers can be chained together through the combination of TimerMasterModeSelection and
 * TimerClockSource options. Master / Slave configurations are dependent on the MCU, so it is important to check the
 * Reference Document for the MCU you are using.
 */
enum class TimerMasterSlaveMode {
    /**
     * @brief The Master/Slave mode is enabled.
     * @details
     * This timer will control other timers that are enabled to listen to TimerMasterModeSelection trigger output as
     * their clock source.
     */
    ENABLE = TIM_MASTERSLAVEMODE_ENABLE,
    /** The Master/Slave mode is disabled. */
    DISABLE = TIM_MASTERSLAVEMODE_DISABLE,
};

/**
 * @brief A timer configuration provides a timer with all configuration variables needed to be set up..
 * @details
 *
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
