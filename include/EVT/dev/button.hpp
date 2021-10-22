#ifndef _EVT_BUTTON_
#define _EVT_BUTTON_

#include <stdint.h>


namespace EVT::core::IO 
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
    Button(GPIO button);

    /**
     * Reads the state of the button 
     * 
     * @return The button state.
     */
    Press readButton();

    /**
     * Confirms a button press due to bouncing signal
     * 
     * @return Confirmation of a press  
     */
    Press debounce();

protected:
    GPIO button;

};

}

#endif
