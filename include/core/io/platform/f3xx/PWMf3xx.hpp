#ifndef _EVT_PWMf3xx_
#define _EVT_PWMf3xx_

#include <stdint.h>

#include <HALf3/stm32f3xx.h>
#include <core/dev/platform/f3xx/Timerf3xx.hpp>
#include <core/io/PWM.hpp>

namespace core::io {

/**
 * @brief A PWM instance for F3xx family STM32s
 *
 * A subclass of both @ref PWM and @ref TimerF3xx. Since PWM on the F3 requires its own instance of a timer, a subclass
 * is used instead of an internal timer variable. This provides more control over the timer, and is more accurate to
 * what PWM is on the F3 (just a timer instance that flips a GPIO pin).
 */
class PWMf3xx : public PWM, public dev::TimerF3xx {
public:
    /**
    * Set up the given pin for PWM usage.
    *
    * @param pin[in] The pin to set up for PWM
    */
    explicit PWMf3xx(Pin pin);

    /**
     * Set the duty cycle for PWM
     * @param[in] dutyCycle The duty cycle to set PWM at.
     */
    void setDutyCycle(uint32_t dutyCycle);

    /**
     * Set the period for PWM.
     * @param[in] period The period for PWM.
     */
    void setPeriod(uint32_t period);

    /**
     * Get the current duty cycle.
     * @return the duty cycle that the PWM instance is currently running at.
     */
    uint32_t getDutyCycle();

    /**
     * Get the current period.
     * @return the period that the PWM instance is currently running at.
     */
    uint32_t getPeriod();

private:
    /**
     * The channel that the timer will run on for this PWM instance
     * */
    uint32_t halTIMChannelID{};
};

} // namespace core::io

#endif
