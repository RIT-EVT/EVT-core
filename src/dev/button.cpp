#include <EVT\dev\button.hpp>
#include <EVT\utils\time.hpp>

namespace EVT::core::DEV {

void button::setButtonState(EVT::core::IO::GPIO::State state) {
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