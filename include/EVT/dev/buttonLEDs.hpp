#ifndef _EVT_buttonLEDs_
#define _EVT_buttonLEDs_

#include <EVT/dev/LED.hpp>
#include <EVT/dev/button.hpp>
#include <EVT/io/GPIO.hpp>


namespace EVT::core::DEV
{

class LED
{
public:
    /**
     * Tells an LED to be on or off
     *
     * @return The state to write to the LED
     */
    LED writeLED();
private:
    /**
     * Read whether an LED is on or off
     *
     * @return ledState Read whether the LED is on or off
     */
    LED readLED();  // May need to split since some buttons may not use LEDs
};

}

#endif
