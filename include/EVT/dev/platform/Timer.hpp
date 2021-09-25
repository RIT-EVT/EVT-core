#ifndef EVT_TIMER_HPP
#define EVT_TIMER_HPP

#include<HALf3/stm32f3xx.h>
#include<HALf3/stm32f3xx_hal_def.h>

namespace EVT::core::DEV {

/**
 * This class will represent an internal general purpose timer device for the STM32.
 * It is capable of triggering interrupts with a given frequency
 */
class Timer{
public:
    /**
     * Starts the given timer and registers the given interrupt pointer to trigger when the timer overflows
     * @param irqHandler[in] The IRQ Handler function pointer.  Sets a new interrupt handler function
     */
    virtual void startTimer(void (*irqHandler)(TIM_HandleTypeDef *htim)) = 0;

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
};

}  // namespace EVT::core::DEV

#endif //EVT_TIMER_HPP
