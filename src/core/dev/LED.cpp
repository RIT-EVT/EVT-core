#include <core/dev/LED.hpp>

namespace core::DEV {

LED::LED(core::IO::GPIO& gpio, LED::ActiveState activeState) : gpio(gpio) {
    this->setState(core::IO::GPIO::State::LOW);
}

void LED::toggle() {
    core::IO::GPIO::State currentState = this->gpio.readPin();

    if (core::IO::GPIO::State::LOW == currentState) {
        this->gpio.writePin(core::IO::GPIO::State::HIGH);
    } else {
        this->gpio.writePin(core::IO::GPIO::State::LOW);
    }
}

void LED::setState(core::IO::GPIO::State state) {
    // if the LED is active high, it's state follows from pin setting
    if (this->activeState == ActiveState::HIGH) {
        this->gpio.writePin(state);
    } else {
        if (state == core::IO::GPIO::State::HIGH) {
            this->gpio.writePin(core::IO::GPIO::State::LOW);
        } else {
            this->gpio.writePin(core::IO::GPIO::State::HIGH);
        }
    }
}

} // namespace core::DEV
