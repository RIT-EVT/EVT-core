#ifndef _EVT_MANAGER_H_
#define _EVT_MANAGER_H_

#include <EVT/io/GPIO.hpp>
#include <EVT/io/PWM.hpp>

#ifdef STM32F302x8
    #include <EVT/io/platform/f3xx/f302x8/GPIOf302x8.hpp>
    #include <EVT/io/platform/f3xx/f302x8/PWMf302x8.hpp>
#endif

namespace EVT::core::IO
{

/**
 * Get an instance of a GPIO pin.
 *
 * @param pin The pin to attach to the GPIO
 * @param direction The direction, either input or output
 */
template<Pin pin>
GPIO& getGPIO(GPIO::Direction direction=GPIO::Direction::OUTPUT) {
    #ifdef STM32F302x8
        static GPIOf302x8 gpioPin(pin, direction);
        return gpioPin;
    #endif
}

/**
 * Get an instance of a PWM pin.
 *
 * @param pin The pin to attach to the PWM.
 */
template<Pin pin>
PWM& getPWM() {
    #ifdef STM32F302x8
        static PWMf302x8 pwm(pin);
        return pwm;
    #endif
}

}  // EVT::core::IO
#endif
