#include <EVT/dev/button.hpp>
#include <EVT/utils/time.hpp>

namespace EVT::core::DEV {
Button::Button(IO::GPIO& gpio, IO::GPIO::State pressedState) : gpio(gpio), pressedState(pressedState) {
    this->timeSinceLastPress = 0;
}

IO::GPIO::State Button::getState() {
    return this->gpio.readPin();
}

bool Button::debounce(uint32_t debounceTime) {
    if (time::millis() - this->timeSinceLastPress > debounceTime) {
        if (this->getState() == pressedState) {
            this->timeSinceLastPress = time::millis();
            return true;
        }
    }

    return false;
}

}// namespace EVT::core::DEV