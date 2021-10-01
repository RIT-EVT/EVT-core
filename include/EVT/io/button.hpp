#ifndef _EVT_BUTTON_
#define _EVT_BUTTON_

#include <stdint.h>

namespace EVT::core::IO {
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Button;

class BUTTON {
   public:
    /**
     * Binary representation of the states the button can be in
     */
    enum class State {
        LOW = 0u,
        HIGH = 1u 
    };

    enum class Press {
        UNPRESSED = 0u,
        PRESSED = 1u
    };

    BUTTON(Button button);


};

}

#endif