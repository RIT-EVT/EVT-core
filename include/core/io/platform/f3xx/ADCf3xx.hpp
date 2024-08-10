#ifndef _EVT_ADCf3xx_
#define _EVT_ADCf3xx_

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal.h>

#include <core/io/ADC.hpp>

namespace EVT::core::IO {

class ADCf3xx : public ADC {
public:
    /**
     * Setup the given pin for ADC usage
     *
     * @param[in] pin The pin to setup for ADC
     */
    ADCf3xx(Pin pin);

    float read();

    uint32_t readRaw();

    float readPercentage();

private:
    // Max number of channels supported by the ADC
    static constexpr uint8_t MAX_CHANNELS = 15;
    // Positive reference voltage of the ADC.  Needs to be updated based on the hardware configuration
    static constexpr float VREF_POS = 3.3;
    // Max value for a 12 bit ADC reading (2^12 - 1)
    static constexpr uint32_t MAX_RAW = 4095;
    /// This is static since the STM32F3xx only has a single ADC which
    /// supports multiple channels. The ADC will be initialized once then
    /// each channel will be added on.
    static ADC_HandleTypeDef halADC;
    /// Static list of all channels supported by the ADC
    static Pin channels[MAX_CHANNELS];
    /// Buffer for DMA where each spot represents the value read in from a
    /// channel
    static uint16_t buffer[MAX_CHANNELS];
    static DMA_HandleTypeDef halDMA;

    /**
     * Initialize the HAL ADC handler. This should only have to be run once
     */
    void initADC(uint8_t num_channels);

    /**
     * Initialize the HAL DMA for the ADC, should only have to be run once
     */
    void initDMA();

    /**
     * Adds an ADC channel to the HAL ADC device.
     *
     * @param rank The "rank" which represents the order in which the channel
     *      was added to the ADC starting at 1
     */
    void addChannel(uint8_t rank);
};

}// namespace EVT::core::IO

#endif
