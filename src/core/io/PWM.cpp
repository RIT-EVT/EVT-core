#include <core/io/PWM.hpp>

namespace core::io {

PWM::PWM(Pin pin) {
    this->pin       = pin;
    this->dutyCycle = 0;
    this->period    = 0;
}

} // namespace core::io
