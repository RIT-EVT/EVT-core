#include <EVT/dev/Thermistor.hpp>

namespace EVT::core::DEV {

Thermistor::Thermistor(EVT::core::IO::ADC& adc, uint32_t (*conversion)(uint32_t)) : adc(adc),
                                                                                    conversion(conversion) {
}

uint32_t Thermistor::getTempCelcius() {
    return conversion(adc.readRaw());
}

}// namespace EVT::core::DEV
