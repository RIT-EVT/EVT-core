#ifndef EVT_TIMER_HPP
#define EVT_TIMER_HPP

#include <cstdint>

/**
 * If AUTO_PRESCALER is sent as a prescaler the timer will calculate its own Prescaler.
 * Yes, this is a negative in an uint32_t, but it ends up just causing an underflow, and making it the max of an
 * uint32_t.
 */
#define AUTO_PRESCALER uint32_t(-1)

namespace core::dev {
/**
 * @brief A timer configuration provides a timer with all configuration variables needed to be set up..
 */
typedef struct {
    /**
     * Specifies how the timer will count.
     * This parameter can be a value of @ref TIM_Counter_Mode
     * Maps to a timer's @ref TIM_Base_InitTypeDef:CounterMode
     */
    uint32_t counterMode{};

    /**
     * Specifies the clock division.
     * This parameter can be a value of @ref TIM_ClockDivision
     * Maps to a timer's @ref TIM_Base_InitTypeDef:ClockDivision
     */
    uint32_t clockDivision{};

    /**
     * Specifies the auto reload preload value, which controls whether @ref TIMx_ARR is buffered or not.
     * This parameter can be a value of @ref TIM_AutoReloadPreload
     * Maps to a timer's @ref TIM_Base_InitTypeDef:AutoReloadPreload
     */
    uint32_t autoReloadPreload{};

    /**
     * Timer clock source
     * This parameter can be a value of @ref TIM_Clock_Source
     * Maps to @ref TIM_ClockConfigTypeDef:ClockSource
     */
    uint32_t clockSource{};

    /**
     * Trigger output (TRGO) selection.
     * This parameter can be a value of @ref TIM_Master_Mode_Selection
     * Maps to @ref TIM_MasterConfigTypeDef:MasterOutputTrigger
     */
    uint32_t masterOutputTrigger{};

    /**
     * Master/slave mode selection.
     * This parameter can be a value of @ref TIM_Master_Slave_Mode
     * Maps to @ref TIM_MasterConfigTypeDef:MasterSlaveMode
     */
    uint32_t masterSlaveMode{};
} TimerConfiguration_t;

class Timer {
public:
    /**
     * Virtual destructor required by C++.
     */
    virtual ~Timer() = default;

    /**
     * Starts the given timer and registers the given interrupt pointer to trigger when the timer overflows
     * @param[in] irqHandler The IRQ Handler function pointer.  Sets a new interrupt handler function
     */
    virtual void startTimer(void (*irqHandler)(void* htim)) = 0;

    /**
     * Starts the given timer using the IRQ Handler already assigned to that timer.
     */
    virtual void startTimer() = 0;

    /**
     * Stops the current timer from running.  Does not complete its current counting sequence.
     */
    virtual void stopTimer() = 0;

    /**
     * Resets the timer counter.
     */
    virtual void reloadTimer() = 0;

    /**
     * Set the clock period for the timer.  Will stop the timer, re-initialize the device with the updated period.
     * You must call startTimer again to continue timer operation.
     *
     * @param[in] clockPeriod the clock period in ticks (ms when using AUTO_PRESCALER).  An interrupt will be triggered
     * at that frequency.
     * @param [in] clockPrescaler the prescaler used by the timer. Divides the system clock frequency to get it within
     * an acceptable range for use with the required period. If set to @ref AUTO_PRESCALER, the function implementation
     * will calculate its own prescaler.
     */
    virtual void setPeriod(uint32_t clockPeriod, uint32_t clockPrescaler = AUTO_PRESCALER) = 0;
};
} // namespace core::dev

#endif // EVT_TIMER_HPP
