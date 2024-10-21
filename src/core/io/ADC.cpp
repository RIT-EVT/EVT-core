#include <core/io/ADC.hpp>

namespace core::io {

ADC::ADC(Pin pin, ADCPeriph adcPeriph) : pin(pin), adcPeriph(adcPeriph) {}

} // namespace core::io
