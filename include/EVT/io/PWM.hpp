#ifndef _EVT_PWM_
#define _EVT_PWM_

#include <stdint.h>

namespace EVT::core::IO {

// Forward declarations:
// The diferent pins are hardware specific. Forward declarations to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

class PWM
{

public:

    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin The pin to setup for PWM
     */
    PWM(Pin pin);

    /**
     * Set the duty cycle for the pin to operate at.
     *
     * @param dutyCycle Duty cycle to set the pin to.
     */
    virtual void setDutyCycle(float dutyCycle) = 0;

    /**
     * Set the period for the PWM.
     *
     * @param period The period of the PWM in micro seconds.
     */
    virtual void setPeriod(uint32_t period) = 0;

    /**
     * Get the current duty cycle.
     *
     * @return The duty cycle the PWM is operating at.
     */
    virtual float getDutyCycle() = 0;

    /**
     * Get the current period.
     *
     * @return The period the PWM is operating at.
     */
    virtual uint32_t getPeriod() = 0;

protected:
    /// The pin the PWM is attached to
    Pin pin;
    /// The duty cycle of the PWM
    float dutyCycle;
    /// The period of the PWM
    uint32_t period;
};


}  // namespace EVT::core::IO

#endif
