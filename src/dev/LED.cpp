#include <EVT/dev/LED.hpp>

namespace EVT::core::DEV {

LED::LED(EVT::core::IO::GPIO& gpio, LED::ActiveState activeState)
    : gpio(gpio) {
    this->setState(EVT::core::IO::GPIO::State::LOW);
}

void LED::toggle() {
    EVT::core::IO::GPIO::State currentState = this->gpio.readPin();

    if (EVT::core::IO::GPIO::State::LOW == currentState) {
        this->gpio.writePin(EVT::core::IO::GPIO::State::HIGH);
    } else {
        this->gpio.writePin(EVT::core::IO::GPIO::State::LOW);
    }
}

void LED::setState(EVT::core::IO::GPIO::State state) {
    // if the LED is active high, it's state follows from pin setting
    if (this->activeState == ActiveState::HIGH) {
        this->gpio.writePin(state);
    } else {
        if (state == EVT::core::IO::GPIO::State::HIGH) {
            this->gpio.writePin(EVT::core::IO::GPIO::State::LOW);
        } else {
            this->gpio.writePin(EVT::core::IO::GPIO::State::HIGH);
        }
    }
}

}  // namespace EVT::core::DEV
