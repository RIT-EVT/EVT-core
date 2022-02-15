#ifndef _EVT_PWM_
#define _EVT_PWM_

#include <stdint.h>

namespace EVT::core::IO {

// Forward declarations:
// The different pins are hardware specific. Forward declarations to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

class PWM {

public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWM(Pin pin);

    /**
     * Set the duty cycle for the pin to operate at.
     *
     * @param dutyCycle[in] Duty cycle to set the pin to.
     */
    virtual void setDutyCycle(uint32_t dutyCycle) = 0;

    /**
     * Set the period for the PWM in seconds.
     *
     * @param period[in] The period of the PWM in seconds.
     */
    virtual void setPeriod(uint32_t period) = 0;

    /**
     * Get the current duty cycle.
     *
     * @return The duty cycle the PWM is operating at.
     */
    virtual uint32_t getDutyCycle() = 0;

    /**
     * Get the current period.
     *
     * @return The period the PWM is operating at in seconds.
     */
    virtual uint32_t getPeriod() = 0;

protected:
    /// The pin the PWM is attached to
    Pin pin;
    /// The duty cycle of the PWM
    uint32_t dutyCycle;
    /// The period of the PWM in microseconds
    uint32_t period;
    /// The amount of microseconds in a second
    const uint32_t MICROSECONDS_IN_SECOND = 1000000;
};

}// namespace EVT::core::IO

#endif
