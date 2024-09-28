#ifndef _EVT_ADC_
#define _EVT_ADC_

#include <stdint.h>

namespace core::io {

// Forward declarations:
// The different pins are hardware specific. Forware declarationsto allow
// at compilation time the decision of which pins should be used.
enum class Pin;
enum class ADCPeriph;

class ADC {

public:
    /**
     * Setup the given pin for ADC (analog to digital) usage
     *
     * @param[in] pin The pin to setup for ADC
     */
    ADC(Pin pin, ADCPeriph adcPeriph);

    /**
     * Reads the current voltage in volts on the ADC
     *
     * @return The voltage in volts
     */
    virtual float read() = 0;

    /**
     * Read the raw value from the ADC
     *
     * @return The raw value from the ADC
     */
    virtual uint32_t readRaw() = 0;

    /**
     * Read the value from the ADC as a percentage of the possible values
     * from 0 to 1 This is based on the maximum possible valie the ADC
     * can read.
     *
     * @return The ADC value as a percentage
     */
    virtual float readPercentage() = 0;

protected:
    /// The pin the ADC is attached to
    Pin pin;
    /// The ADC being used
    ADCPeriph adcPeriph;
};

} // namespace core::io

#endif
