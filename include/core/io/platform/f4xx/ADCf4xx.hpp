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
    TIM_HandleTypeDef htim8;

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
    struct ADC_State_t {
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

    static ADC_State_t adcArray[NUM_ADCS];

    /**
     * Checks if the channel that is being initialized supports the ADC peripheral that it is being initialized on.
     *
     * @param periph the ADC peripheral being used
     * @param channel the channel trying to be initialized
     * @return true if channel is supported by ADCPeriph
     */
    inline bool checkSupport(ADCPeriph periph, uint32_t channel);

    /**
     * Returns the ADC number that is in use. Depends on the ADCPeriph enum to check
     *
     * @return The adc number that is being used in this specific object
     */
    inline uint8_t getADCNum();

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

    /**
     * Initializes Timer 8 to control ADC conversion frequency
     */
    void InitTimer();
};

} // namespace core::io

#endif
