#ifndef _EVT_BUTTON_
#define _EVT_BUTTON_

#include <stdint.h>


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

enum class Press 
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
    // bool readButton(); 

    /**
     * Time how long the button is in a high state
     * 
     * @return Time in milliseconds
     */
    uint32_t debounce();

    bool setDebounce(uint32_t debounce);

private:
    GPIO button;

};

}

#endif
