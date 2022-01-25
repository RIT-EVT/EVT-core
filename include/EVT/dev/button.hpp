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
         * Enum to handle the button trigger states
         *
         */
    enum class LogicLevelOnPress {
        LOW = 0u,
        HIGH = 1u,
        UNDETERMINED = 2u,
    };

    /**
         * Create an instance of the LED based on the given GPIO pin.
         *
         * @param gpio[in] GPIO pin
         * @param logicLevelOnPress[in] Represents if the button is active high or active low
         */
    Button(EVT::core::IO::GPIO& gpio, Button::LogicLevelOnPress logicLevelOnPress);

    /**
         * Reads the state of the button
         *
         * @return The button state.
         */
    IO::GPIO::State readButton();

    /**
         * Determine whether a button was pressed
         *
         * @return Confirmation of a button press
         */
    DEV::Button::LogicLevelOnPress debounce();

    /**
         * @brief Set the Button State object
         *
         * @return Boolean if button has been in logic level high for a certain amount of time
         */
    bool debounceState(uint32_t debounce);

private:
    EVT::core::IO::GPIO& gpio;
    LogicLevelOnPress logicLevelOnPress;
};

}// namespace EVT::core::DEV

#endif
