#ifndef _EVT_THERMISTOR_
#define _EVT_THERMISTOR_

#include <stdint.h>

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
    Thermistor(EVT::core::IO::ADC& adc, uint32_t (*conversion)(uint32_t));

    /**
     * Return the temperature in milli celcius of the thermistor
     */
    uint32_t getTempCelcius();

private:
    /// The ADC interface to read from
    EVT::core::IO::ADC& adc;
    /// Funtion that converts raw ADC values into milli Celcius
    uint32_t (*conversion)(uint32_t);
};

}



#endif
