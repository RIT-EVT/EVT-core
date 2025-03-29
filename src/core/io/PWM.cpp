#include <core/io/PWM.hpp>
#include <core/manager.hpp>

namespace core::io {

PWM::PWM(Pin pin) : timer(dev::getTimer<dev::MCUTimer::Timer2>(100, {})) {
    this->pin       = pin;
    this->dutyCycle = 0;
    this->period    = 0;
}

} // namespace core::io
