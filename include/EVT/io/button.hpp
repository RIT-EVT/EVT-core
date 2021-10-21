#ifndef _EVT_BUTTON_
#define _EVT_BUTTON_

#include <stdint.h>


namespace EVT::core::IO {
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class GPIO;

class Button {
   public:
    /**
     * Enum to handle the button trigger states
     *
     */

    enum class Press {
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
    virtual Press readButton();

    /**
     * Confirms a button press due to bouncing signal
     * 
     * @return Confirmation of a press  
     */
    virtual void debounce();


    /**
     * Tells an LED to be on or off
     * 
     * @return The state to write to the LED
     */
    virtual void writeLED();

    /**
     * Read whether an LED is on or off
     * 
     * @return ledState Read whether the LED is on or off 
     */
    virtual Press readLED(); // May need to split since some buttons may not use LEDs


    protected:
        GPIO button;

};

}

#endif