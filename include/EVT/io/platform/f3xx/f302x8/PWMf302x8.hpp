#ifndef _EVT_PWMf302x8_
#define _EVT_PWMf302x8_

#include <stdint.h>

#include <EVT/io/PWM.hpp>
#include <EVT/io/pin.hpp>
#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class PWMf302x8 : public PWM {
public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWMf302x8(Pin pin);

    /**
     * Set the duty cycle for the pin to operate at.
     *
     * @param dutyCycle[in] Duty cycle to set the pin to.
     */
    void setDutyCycle(float dutyCycle);

    /**
     * Set the period for the PWM.
     *
     * @param period[in] The period of the PWM in seconds.
     */
    void setPeriod(float period);

    /**
     * Get the current duty cycle.
     *
     * @return The duty cycle the PWM is operating at.
     */
    float getDutyCycle();

    /**
     * Get the current period.
     *
     * @return The period the PWM is operating at.
     */
    uint32_t getPeriod();

private:
    /// HAL timer representation
    TIM_HandleTypeDef halTIM;
    /// Channel identification
    uint32_t halTIMChannelID;
    /// HAL channel representation
    TIM_OC_InitTypeDef halChannel;
};

}// namespace EVT::core::IO

#endif
