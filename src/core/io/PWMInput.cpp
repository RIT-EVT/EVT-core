#include <core/io/PWMInput.hpp>

namespace core::io {

PWMInput::PWMInput(Pin pin) { // core::io::Pin pin
    this->pin       = pin;
    this->dutyCycle = 0;
    this->period    = 0;
    this->frequency = 0;
}

} // namespace core::io
