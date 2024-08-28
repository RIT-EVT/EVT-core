#include <core/dev/LED.hpp>

namespace core::dev {

LED::LED(core::io::GPIO& gpio, LED::ActiveState activeState) : gpio(gpio) {
    this->setState(core::io::GPIO::State::LOW);
}

void LED::toggle() {
    core::io::GPIO::State currentState = this->gpio.readPin();

    if (core::io::GPIO::State::LOW == currentState) {
        this->gpio.writePin(core::io::GPIO::State::HIGH);
    } else {
        this->gpio.writePin(core::io::GPIO::State::LOW);
    }
}

void LED::setState(core::io::GPIO::State state) {
    // if the LED is active high, it's state follows from pin setting
    if (this->activeState == ActiveState::HIGH) {
        this->gpio.writePin(state);
    } else {
        if (state == core::io::GPIO::State::HIGH) {
            this->gpio.writePin(core::io::GPIO::State::LOW);
        } else {
            this->gpio.writePin(core::io::GPIO::State::HIGH);
        }
    }
}

} // namespace core::dev
