#ifndef _EVT_THERMISTOR_
#define _EVT_THERMISTOR_

#include <EVT/io/ADC.hpp>

namespace EVT::core::DEV {

/**
 * Handles representing any device that can produce temperature reading based
 * on incoming ADC values.
 */
class Thermistor {
public:
    /**
     * Create a new thermistor which will use the provided ADC and
     * pass those values through the conversion to produce a temperature
     * reading.
     */
    Thermistor(EVT::core::IO::ADC& adc, float (*conversion)(float));

    /**
     * Return the temperature in celcius of the thermistor
     */
    float getTempCelcius();

private:
    /// The ADC interface to read from
    EVT::core::IO::ADC& adc;
    /// Function which can take the voltage input from the ADC and produce
    /// a temperature. The function should return the temperature in Celcius
    float (*conversion)(float);
};

}



#endif
