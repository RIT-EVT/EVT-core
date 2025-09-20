#ifndef _EVT_DACF3XX_H
#define _EVT_DACF3XX_H

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal.h>

#include <core/io/DAC.hpp>

namespace core::io {

enum class DACPeriph {
    ONE
};

class DACf3xx : public DACBase {
public:
    /**
     * Setup the given pin for DAC usage
     *
     * @param[in] pin The pin to setup for DAC
     * @param[in] dacPeriph The DAC peripheral being used
     */
    DACf3xx(Pin pin, DACPeriph dacPeriph);

    void setValue(uint32_t value) override;
    uint32_t getValue() const override;
    void setVoltage(float voltage) override;
    float getVoltage() const override;

    DAC_HandleTypeDef* getHalDac() { return &halDac; }

private:
    static constexpr uint32_t MAX_RAW = 4095;
    static constexpr float VREF_POS = 3.3;
    uint32_t currentValue = 0;
    DAC_HandleTypeDef halDac;

    /**
     * Bit packed struct to contain the channel along with the DAC peripherals the channel supports
     *
     * dac1: 1 bit. Support for DAC1 peripheral. 1 for supported, 0 for not supported.
     * channel: 5 bits. The STM32 DAC channel value with said supported DAC peripherals
     */
    struct Channel_Support {
        uint8_t dac1    : 1;
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

    void initDAC();
    void initGPIO();
};

} // namespace core::io

#endif // _EVT_DACF3XX_H

