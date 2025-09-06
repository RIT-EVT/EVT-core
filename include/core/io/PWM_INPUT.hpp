
#ifndef EVT_PWM_INPUT_HPP
#define EVT_PWM_INPUT_HPP

#include <stdint.h>

namespace core::io {

// Different pins are hardware specific
enum class Pin;

class PWM_INPUT {

public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param[in] pin The pin to setup for PWM
     */
    PWM_INPUT(Pin pin);

    /**
     * Get the current duty cycle.
     *
     * @return The duty cycle the PWM input is operating at.
     */
    virtual uint32_t getDutyCycle() = 0;

    /**
     * Get the current period.
     *
     * @return The period the PWM input is operating at in microseconds.
     */
    virtual uint32_t getPeriod() = 0;

    /**
     * Get the current period.
     *
     * @return The frequency the PWM input is operating at in Hz.
     */
    virtual uint32_t getFrequency() = 0;

protected:
    /// The pin PWM input is on
    Pin pin;
    /// The duty cycle of the PWM input
    uint32_t dutyCycle;
    /// The period of the PWM input
    uint32_t period;
    /// The frequency of the PWM input
    uint32_t frequency;
};

} // namespace core::io

#endif
