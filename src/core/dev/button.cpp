#include <core/dev/button.hpp>
#include <core/utils/time.hpp>

namespace core::dev {
Button::Button(io::GPIO& gpio, io::GPIO::State pressedState) : gpio(gpio), pressedState(pressedState) {
    this->timeSinceLastPress = 0;
}

io::GPIO::State Button::getState() {
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

} // namespace core::dev