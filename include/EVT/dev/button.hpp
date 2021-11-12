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


    enum class ActiveState 
    {
        HIGH = 0u,
        LOW = 1u,
    };

    /**
     * Create an instance of the LED based on the given GPIO pin.
     *
     * @param gpio[in] GPIO pin
     * @param activeState[in] Represents if the LED is active high or active low
     */
    Button(EVT::core::IO::GPIO& gpio, ActiveState activeState);

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
