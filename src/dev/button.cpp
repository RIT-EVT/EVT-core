#include <EVT/dev/button.hpp>
#include <HALf3/stm32f3xx_hal.h>

namespace EVT::core::DEV {
    Button::Button(IO::GPIO &gpio) : gpio(gpio) {
        this->gpio = gpio;
    }

    IO::GPIO::State Button::getState() {
        return this->gpio.readPin();
    }
}// namespace EVT::core::DEV
