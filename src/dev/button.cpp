#include <EVT/dev/button.hpp>

namespace EVT::core::DEV {
Button::Button(EVT::core::IO::GPIO& gpio, Button::ActiveState activeState) : gpio(gpio), activeState(activeState) {
}




IO::GPIO::State Button::readButton() {
    if (this->activeState == Button::ActiveState::HIGH) {
        return this->gpio.readPin();
    } else {
        auto state = gpio.readPin();
        if (state == EVT::core::IO::GPIO::State::HIGH) {
            return EVT::core::IO::GPIO::State::LOW;
        } else {
            return EVT::core::IO::GPIO::State::HIGH;
        }
    }
}

}  // namespace EVT::core::DEV