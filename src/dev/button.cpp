#include <EVT/dev/button.hpp>


namespace EVT::core::DEV {
Button::Button(EVT::core::IO::GPIO& gpio, Button::LogicLevelOnPress logicLevelOnPress) : gpio(gpio), logicLevelOnPress(logicLevelOnPress) {
}

IO::GPIO::State Button::readButton() {
    if (this->logicLevelOnPress == Button::LogicLevelOnPress::HIGH) {
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