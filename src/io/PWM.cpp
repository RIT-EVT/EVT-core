#include <EVT/io/PWM.hpp>

namespace EVT::core::IO {

PWM::PWM(Pin pin) {
    this->pin = pin;
    this->dutyCycle = 0;
    this->period = 0;
}

}// namespace EVT::core::IO
