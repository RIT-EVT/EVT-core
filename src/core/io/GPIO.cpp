#include <core/io/GPIO.hpp>

namespace EVT::core::IO {

// Setup instance variables
GPIO::GPIO(Pin pin) {
    this->pin = pin;
}

GPIO::GPIO(Pin pin, Direction direction, Pull pull) {
    this->pin = pin;
    this->direction = direction;
    this->pull = pull;
}

}// namespace EVT::core::IO
