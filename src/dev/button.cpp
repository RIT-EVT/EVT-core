#include <EVT/dev/button.hpp>
#include <EVT/utils/time.hpp>

namespace EVT::core::DEV {
Button::Button(IO::GPIO& gpio) : gpio(gpio) {
    this->gpio = gpio;
    this->timeSinceLastPress = 0;
}

IO::GPIO::State Button::getState() {
    return this->gpio.readPin();
}

bool Button::debounce(uint32_t debounceTime) {
    if (time::millis() - this->timeSinceLastPress > debounceTime) {
        if (this->getState() == IO::GPIO::State::HIGH) {
            this->timeSinceLastPress = time::millis();
            return true;
        }
    }

    return false;
}

}// namespace EVT::core::DEV