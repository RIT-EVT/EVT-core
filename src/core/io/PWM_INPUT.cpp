#include <core/io/PWM_INPUT.hpp>

namespace core::io {

PWM_INPUT::PWM_INPUT(core::io::Pin pin) {
    this->pin       = pin;
    this->dutyCycle = 0;
    this->period    = 0;
    this->frequency = 0;
}

} // namespace core::io
