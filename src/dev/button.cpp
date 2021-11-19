#include <EVT\dev\button.hpp>
#include <EVT\utils\time.hpp>

namespace EVT::core::DEV {

Button::Button(EVT::core::IO::GPIO& gpio, LED::ActiveState activeState)
    : gpio(gpio) {
    this->setState(EVT::core::IO::GPIO::State::LOW);
}
)



    void LED::readButtonState(EVT::core::IO::GPIO::State state) {
        if (this->activeState == Button::ActiveState::HIGH) {
            return this->gpio.readPin(state);
        } else {
            auto state = gpio.readPin();
            if (state == EVT::core::IO::GPIO::State::HIGH) {
                return EVT::core::IO::GPIO::State::LOW;
            } else {
                return EVT::core::IO::GPIO::State::HIGH;
            }
        }
    }
}
}

}  // namespace EVT::core::DEV