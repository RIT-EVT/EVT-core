#include <EVT/dev/Thermistor.hpp>

namespace EVT::core::DEV {

Thermistor::Thermistor(EVT::core::IO::ADC& adc, float (*conversion)(float)) :
    adc(adc),
    conversion(conversion) {

}

float Thermistor::getTempCelcius() {
    return conversion(adc.read());
}

}
