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
        Button(IO::GPIO &gpio);

        IO::GPIO::State getState();


    private:
        IO::GPIO &gpio;

    }; // namespace EVT::core::DEV
}
#endif
