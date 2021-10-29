#ifndef _EVT_buttonLEDs_
#define _EVT_buttonLEDs_

#include <stdint.h>
#include <EVT/dev/LED.hpp>
#include <EVT/dev/button.hpp>
#include <EVT/io/GPIO.hpp>


namespace EVT::core::DEV
{

class ButtonLEDs : Button
{

public:
    /**
     * Tells an LED to be on or off
     *
     * @return The state to write to the LED
     */
    void writeLED();
    
private:
    Array<buttonLEDs> 

     /**
      * Read whether an LED is on or off
      *
      * @return ledState Read whether the LED is on or off
      */
    bool readLED();  // May need to split since some buttons may not use LEDs
};

}

#endif
