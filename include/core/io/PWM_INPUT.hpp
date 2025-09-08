
#ifndef EVT_PWM_INPUT_HPP
#define EVT_PWM_INPUT_HPP

#include <stdint.h>

namespace core::io {

// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

class PWM_INPUT {

public:
    /**
     * Setup the given pin for PWM_INPUT usage.
     *
     * @param[in] pin The pin to setup for PWM_INPUT
     */
    PWM_INPUT(Pin pin);

    /**
     * Get the current duty cycle.
     *
     * @return The duty cycle of the PWM signal being read, as a percentage.
     */
    virtual uint32_t getDutyCycle() = 0;

    /**
     * Get the current period.
     *
     * @return The period the PWM signal being read is operating at in timer ticks.
     */
    virtual uint32_t getPeriod() = 0;

    /**
     * Get the current frequency.
     *
     * @return The frequency the PWM signal being read is operating at in Hz.
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
