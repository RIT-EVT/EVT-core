#include <core/io/ADC.hpp>

namespace core::io {

ADC::ADC(Pin pin, ADCPeriph adcPeriph) {
    this->pin = pin;
    this->adcPeriph = adcPeriph;
}

} // namespace core::io
