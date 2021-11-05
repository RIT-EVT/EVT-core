#ifndef _EVT_BUTTON_
#define _EVT_BUTTON_

#include <stdint.h>
#include <EVT/io/GPIO.hpp>

namespace EVT::core::DEV 
{
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class GPIO;
bool activeHigh;

class Button 
{
public:
    /**
     * Enum to handle the button trigger states
     *
     */


    enum class ButtonState 
    {
        UNPRESSED = 0u,
        PRESSED = 1u,
    };

    /**
     * Create a new button interface
     *
     */
    Button(GPIO button, uint32_t debounce);

    /**
     * Reads the state of the button 
     * 
     * @return The button state.
     */
    bool readButton();

    /**
     * Time how long the button is in a high state
     * 
     * @return Time in milliseconds
     */
    uint32_t debounce();

    /**
     * @brief Set the Button State object
     * 
     * @return Boolean if button has been in active high for a certain amount of time
     */
    bool debounceState(uint32_t debounce);


    /**
     * Set the Button State object
     * 
     * @param debounceState 
     */
    void setButtonState(bool debounceState)

private:
    GPIO button;
    int state;

};

}

#endif
