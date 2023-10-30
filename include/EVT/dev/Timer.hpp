#ifndef EVT_TIMER_HPP
#define EVT_TIMER_HPP

namespace EVT::core::DEV {

/**
 * This class will represent an internal general purpose timer device for the STM32.
 * It is capable of triggering interrupts with a given frequency
 */
class Timer {
public:
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
     * @param[in] clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    virtual void setPeriod(uint32_t clockPeriod) = 0;
};

}// namespace EVT::core::DEV

#endif//EVT_TIMER_HPP
