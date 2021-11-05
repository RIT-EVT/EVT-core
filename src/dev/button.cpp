#include <EVT\dev\button.hpp>
#include <EVT\utils\time.hpp>

namespace EVT::core::DEV {

// Button::Button(GPIO button, uint32_t debounce) : gpio(gpio) {

// }

uint32_t button::debounce() {



    return debounce_val
}


bool button::debounceState(uint32_t debounce_val) {
    if debounce_val > 10 {
        return true
    } else {
        return false
    }

}

bool button::readButton(class ButtonState) {
    if ButtonState == PRESSED {
        return true
    } else {
        return false
    }
}

}  // namespace EVT::core::DEV