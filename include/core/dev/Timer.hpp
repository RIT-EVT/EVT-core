#ifndef EVT_TIMER_HPP
#define EVT_TIMER_HPP

#include <cstdint>

/**
 * If AUTO_PRESCALER is sent as a prescaler the timer will calculate its own Prescaler in the setPeriod function.
 * The automatic prescaler is calculated by dividing the system clock speed by 1000, then subtracting 1.
 *
 * Yes, this is a negative in an uint32_t, but it ends up just causing an underflow, and making it the max of an
 * uint32_t.
 */
#define AUTO_PRESCALER uint32_t(-1)

namespace core::dev {
class Timer {
public:
    /**
     * Virtual destructor required by C++.
     */
    virtual ~Timer() = default;

    /**
     * Starts the given timer and registers the given interrupt pointer to trigger when the timer overflows
     * @param[in] irqHandler The IRQ Handler function pointer.  Sets a new interrupt handler function
     * @param[in] context The context to be sent to the irqHandler when it is called.
     */
    virtual void startTimer(void (*irqHandler)(void* context, void* htim), void* context) = 0;

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
