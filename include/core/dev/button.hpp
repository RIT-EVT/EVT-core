#ifndef _EVT_BUTTON_
#define _EVT_BUTTON_

#include <core/io/GPIO.hpp>
#include <stdint.h>

namespace EVT::core::DEV {
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.

class Button {
public:
    /**
     * Create an instance of the Button based on the given GPIO pin.
     *
     * @param gpio[in] GPIO pin
     * @param pressedState[in] Which GPIO state indicates the button is pressed
     */
    Button(IO::GPIO& gpio, IO::GPIO::State pressedState = IO::GPIO::State::HIGH);

    /**
     * Gets the button's GPIO Pin state
     *
     * @return The state of the pin
     */
    IO::GPIO::State getState();

    /**
     * Confirms a button press based on a user defined debounce time
     *
     * @param debounceTime
     * @return Confirmation of a button press
     */
    bool debounce(uint32_t debounceTime);

private:
    /** The GPIO pin that the button is connected to */
    IO::GPIO& gpio;

    /** The time since the button was last pressed */
    uint32_t timeSinceLastPress;

    /** The GPIO state that means the button is being pressed */
    IO::GPIO::State pressedState;

};// namespace EVT::core::DEV
}// namespace EVT::core::DEV
#endif
