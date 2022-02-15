#ifndef _EVT_BUTTON_
#define _EVT_BUTTON_

#include <EVT/io/GPIO.hpp>
#include <stdint.h>

namespace EVT::core::DEV {
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.

class Button {
public:
    /**
     * @brief Construct a new Button object
     * @param gpio
     */
    Button(IO::GPIO& gpio);

    /**
     * @brief Gets the button's GPIO Pin state
     * @return The state of the pin
     */
    IO::GPIO::State getState();

    /**
     * @brief Confirms a button press
     * based on a user defined debounce time
     * @param debounceTime
     * @return Confirmation of a button press
     */
    bool debounce(int debounceTime);

private:
    // The GPIO pin that the button is connected to
    IO::GPIO& gpio;

    // The time since the button was last pressed
    int timeSinceLastPress;

};// namespace EVT::core::DEV
}// namespace EVT::core::DEV
#endif
