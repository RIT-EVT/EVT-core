#ifndef EVT_TIMER_HPP
#define EVT_TIMER_HPP

namespace EVT::core::DEV {

/**
 * This class will represent an internal general purpose timer device for the STM32.
 * It is capable of triggering interrupts with a given frequency
 */
class Timer{
public:
    /**
     * Starts the given timer and registers the given interrupt pointer to trigger when the timer overflows
     * @param irqHandler The IRQ Handler function pointer
     */
    virtual void startTimer(void (*irqHandler)()) = 0;
    // TODO: Determine if we want timer to always act on repeat, or if we want one shot timer
    // TODO: Implement to work for any General Purpose Timers on the device instead of using a default one

    /**
     * Stops the current timer from running.  Does not complete its current counting sequence.
     */
    virtual void stopTimer() = 0;
};

}  // namespace EVT::core::DEV

#endif //EVT_TIMER_HPP
