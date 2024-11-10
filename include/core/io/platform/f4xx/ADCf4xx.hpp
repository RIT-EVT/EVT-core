#ifndef _EVT_ADCf4xx_
#define _EVT_ADCf4xx_

#include <HALf4/stm32f4xx.h>
#include <HALf4/stm32f4xx_hal.h>

#include <core/io/ADC.hpp>

namespace core::io {

enum class ADCPeriph {
    ONE,
    TWO,
    THREE
};

class ADCf4xx : public ADC {
public:
    /**
     * Setup the given pin for ADC usage
     *
     * @param[in] pin The pin to setup for ADC
     * @param[in] adcPeriph The ADC peripheral being used
     */
    ADCf4xx(Pin pin, ADCPeriph adcPeriph);

    float read();

    uint32_t readRaw();

    float readPercentage();

private:
    // Max number of channels supported by the ADC
    static constexpr uint8_t MAX_CHANNELS = 16;
    // Number of supported ADC Peripherals
    static constexpr uint8_t NUM_ADCS = 3;
    // Positive reference voltage of the ADC.  Needs to be updated based on the hardware configuration
    static constexpr float VREF_POS = 3.3;
    // Max value for a 12 bit ADC reading (2^12 - 1)
    static constexpr uint32_t MAX_RAW = 4095;
    // Flag to indicate if the timer has been initialized
    static bool timerInit;
    // Timer handle for TIM8, used to configure and control the timer instance
    static TIM_HandleTypeDef htim8;

    /**
     * Structure to represent the state of an ADC instance.
     *
     * This structure holds the configuration and current state for an ADC instance, including the
     * HAL handle for the ADC and DMA, channel configurations, and data buffers for readings.
     *
     * halADC: HAL handle for configuring and controlling the ADC peripheral.
     * rank: The ADC channel rank in the sequence (starts at 1).
     * isADCInit: Flag to indicate whether the ADC has been initialized.
     * channels: Array of pins mapped to ADC channels. Initialized to all Pin::DUMMY.
     *             The array size is defined by MAX_CHANNELS.
     * buffer: Array to store ADC values for each channel, indexed according to the channels array.
     *           Each entry corresponds to a specific ADC channel.
     * halDMA: HAL handle for configuring and controlling DMA for the ADC.
     */
    struct ADC_State {
        ADC_HandleTypeDef halADC      = {0};
        uint8_t rank                  = 1;
        bool isADCInit                = false;
        Pin channels[MAX_CHANNELS]    = {Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY,
                                         Pin::DUMMY};
        uint16_t buffer[MAX_CHANNELS] = {0};
        DMA_HandleTypeDef halDMA      = {0};
    };

    /**
     * Bit packed struct to contain the channel along with the ADC peripherals the channel supports
     *
     * adc1: 1 bit. Support for ADC1 peripheral. 1 for supported, 0 for not supported.
     * adc2: 1 bit. Support for ADC2 peripheral. 1 for supported, 0 for not supported.
     * adc3: 1 bit. Support for ADC3 peripheral. 1 for supported, 0 for not supported.
     * channel: 5 bits. The STM32 ADC channel value with said supported ADC peripherals
     */
    struct Channel_Support {
        uint8_t adc1     : 1;
        uint8_t adc2     : 1;
        uint8_t adc3     : 1;
        uint32_t channel : 5;
    };

    // Array of all ADC peripheral states
    static ADC_State adcArray[NUM_ADCS];
    // The ADC peripheral state of the current object
    ADC_State& adcState;
    // The number ADC peripheral which is being used in the current object
    uint8_t adcNum;

    /**
     * Check if the channel that is being initialized supports the ADC peripheral that it is being initialized on.
     *
     * @param periph the ADC peripheral being used
     * @param channelStruct the struct of the channel with supports to test
     * @return true if channel is supported by the ADC peripheral, false otherwise
     */
    static bool checkSupport(ADCPeriph periph, Channel_Support channelStruct);

    /**
     * Return the ADC number that is in use
     *
     * @return The adc number that is being used in this specific object
     */
    static uint8_t getADCNum(ADCPeriph periph);

    /**
     * Initialize the HAL ADC handler
     */
    void initADC(uint8_t num_channels);

    /**
     * Initialize the HAL DMA for the ADC
     */
    void initDMA();

    /**
     * Add an ADC channel to the HAL ADC device.
     *
     * @param rank The "rank" which represents the order in which the channel
     *      was added to the ADC starting at 1
     */
    void addChannel(uint8_t rank);

    /**
     * Initialize Timer 8 to send Update Events, which the ADC listens for to do a conversion
     * aka controls ADC conversion frequency
     */
    static void initTimer();
};

} // namespace core::io

#endif
