#include <core/dev/Thermistor.hpp>

namespace core::DEV {

Thermistor::Thermistor(core::IO::ADC& adc, uint32_t (*conversion)(uint32_t)) : adc(adc), conversion(conversion) {}

uint32_t Thermistor::getTempCelcius() { return conversion(adc.readRaw()); }

uint32_t Thermistor::getRawADC() { return adc.readRaw(); }

} // namespace core::DEV
