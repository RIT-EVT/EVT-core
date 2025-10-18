#ifndef _EVT_DACF4XX_H
#define _EVT_DACF4XX_H

#include <HALf4/stm32f4xx.h>
#include <HALf4/stm32f4xx_hal.h>

#include <core/io/DAC.hpp>

namespace core::io {

enum class DACPeriph {
    ONE,
    TWO
};

class DACf4xx : public DACBase {
public:
    /**
     * Setup the given pin for DAC usage
     *
     * @param[in] pin The pin to setup for DAC
     * @param[in] dacPeriph The DAC peripheral being used
     */
    DACf4xx(Pin pin, DACPeriph dacPeriph);

    void setValue(uint32_t value) override;
    uint32_t getValue() const override;
    void setVoltage(float voltage) override;
    float getVoltage() const override;

    /// HAL DAC handle for STM32 operations (public for interrupt access)
    DAC_HandleTypeDef halDac;

private:
    /// Maximum raw DAC value (12-bit resolution)
    static constexpr uint32_t MAX_RAW = 4095;
    /// Reference voltage for DAC output calculation
    static constexpr float VREF_POS   = 3.3;
    /// Current DAC output value
    uint32_t currentValue             = 0;
    /// DAC channel for this instance
    uint32_t channel                  = 0;

    /**
     * Bit packed struct to contain the channel along with the DAC peripherals the channel supports
     *
     * dac1: 1 bit. Support for DAC1 peripheral. 1 for supported, 0 for not supported.
     * dac2: 1 bit. Support for DAC2 peripheral. 1 for supported, 0 for not supported.
     * channel: 5 bits. The STM32 DAC channel value with said supported DAC peripherals
     */
    struct Channel_Support {
        uint8_t dac1    : 1;
        uint8_t dac2    : 1;
        uint8_t channel : 5;
    };

    /**
     * Check if the channel that is being initialized supports the DAC peripheral that it is being initialized on.
     *
     * @param periph the DAC peripheral being used
     * @param channelStruct the struct of the channel with supports to test
     * @return true if channel is supported by the DAC peripheral, false otherwise
     */
    static bool checkSupport(DACPeriph periph, Channel_Support channelStruct);

    /**
     * Initialize the DAC peripheral with proper configuration
     */
    void initDAC();

    /**
     * Initialize GPIO pins for DAC functionality
     */
    void initGPIO();

    /**
     * Determine the DAC channel based on the pin
     * @return The STM32 DAC channel value
     */
    uint32_t getChannelFromPin();

    /**
     * Get channel support information for a given pin
     * @param pin The pin to get support information for
     * @return Channel_Support struct with DAC peripheral support information
     */
    static Channel_Support getChannelSupport(Pin pin);
};

} // namespace core::io

#endif // _EVT_DACF4XX_H
