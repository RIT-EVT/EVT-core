#ifndef _EVT_DAC_
#define _EVT_DAC_

#include <stdint.h>

namespace core::io {

// Forward declarations:
// The different pins are hardware specific. Forward declarations to allow
// at compilation time the decision of which pins should be used.
enum class Pin;
enum class DACPeriph;

class DACBase {

public:
    /**
     * Setup the given pin for DAC (digital to analog) usage
     *
     * @param[in] pin The pin to setup for DAC
     * @param[in] dacPeriph The DAC peripheral being used
     */
    DACBase(Pin pin, DACPeriph dacPeriph);

    /**
     * Set the DAC output value
     * @param value The digital value to convert (0-4095 for 12-bit DAC)
     */
    virtual void setValue(uint32_t value) = 0;

    /**
     * Get the current DAC output value
     * @return The current digital value being output
     */
    virtual uint32_t getValue() const = 0;

    /**
     * Set the DAC output voltage directly
     * @param voltage The desired output voltage in volts
     */
    virtual void setVoltage(float voltage) = 0;

    /**
     * Get the current DAC output voltage
     * @return The current output voltage in volts
     */
    virtual float getVoltage() const = 0;

protected:
    /// The pin the DAC is attached to
    Pin pin;
    /// The internal DAC being used
    DACPeriph dacPeriph;
};

} // namespace core::io

// Convenience typedef - use different name to avoid macro conflict
namespace core::io {
    using DigitalToAnalogConverter = DACBase;
}

#endif // _EVT_DAC_

