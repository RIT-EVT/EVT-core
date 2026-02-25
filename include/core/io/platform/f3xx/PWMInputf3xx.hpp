
#ifndef EVT_PWMINPUTF3XX_HPP
#define EVT_PWMINPUTF3XX_HPP

#include <HALf3/stm32f3xx.h>

#include <core/io/PWMInput.hpp>
#include <core/io/pin.hpp>
#include <stdint.h>

namespace core::io {

/**
 * PWM input for f3
 * Measures the duty cycle, frequency, and period of a PWM signal on a pin
 */
class PWMInputf3xx : public PWMInput {
public:
    /**
     * Setup the given pin for PWM Input usage.
     *
     * @param pin[in] The pin to setup for PWM Input Capture
     */
    PWMInputf3xx(Pin pin);

    uint8_t getDutyCycle();

    uint32_t getPeriod();

    uint32_t getFrequency();

    /// Called from HAL_TIM_IC_CaptureCallback to update duty cycle, period, and frequency
    void handleCapture(TIM_HandleTypeDef* htim);

    /// Provides access to HAL handle for IRQ forwarding
    TIM_HandleTypeDef* getHandle();

private:
    /// HAL timer representation
    TIM_HandleTypeDef halTIM; // hal timer handle
    /// Channel for rising edge measurement
    uint32_t directChannel;
    /// Channel for falling edge measurement
    uint32_t indirectChannel;
    /// active channel
    uint32_t activeChannel;
};

} // namespace core::io

#endif
