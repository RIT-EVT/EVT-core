#include <EVT/dev/button.hpp>
#include <HALf3/stm32f3xx_hal.h>

namespace EVT::core::DEV {
    Button::Button(IO::GPIO &gpio) : gpio(gpio) {
        this->gpio = gpio;
        this->timeSinceLastPress = 0;
    }

    IO::GPIO::State Button::getState() {
        return this->gpio.readPin();
    }

    bool Button::debounce(int debounceTime) {
        if (HAL_GetTick() - this->timeSinceLastPress > debounceTime) {
            if (this->getState() == IO::GPIO::State::HIGH) {
                this->timeSinceLastPress = HAL_GetTick();
                return true;
            }
        }

        return false;

    }


}// namespace EVT::core::DEV
