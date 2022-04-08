#ifndef EVT_ADCMUX_HPP
#define EVT_ADCMUX_HPP

#include <EVT/io/GPIO.hpp>

namespace EVT::core::DEV {

    class ADCMux {

    private:
        ///Pin States
        EVT::core::IO::GPIO::State low = EVT::core::IO::GPIO::State::LOW;
        EVT::core::IO::GPIO::State high = EVT::core::IO::GPIO::State::HIGH;

        ///Control pins
        IO::GPIO& ctrlGPIO1;
        IO::GPIO& ctrlGPIO2;

        ///Input from ADC
        IO::ADC& adc;

    public:
        /**
         * Create an instance of ADCMux
         * @param ctrlGPIO1
         * @param ctrlGPIO2
         * @param adc
         */
        ADCMux(EVT::core::IO::GPIO& ctrlGPIO1, EVT::core::IO::GPIO& ctrlGPIO2, IO::ADC& adc);

        /**
         * Get ADC raw input
         * @return raw input
         */
        uint32_t getInput();

        /**
         * Set control pins based on ADC number
         * @param number
         */
        void setOutputPin(uint8_t number);
    };
}

#endif

