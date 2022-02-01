#include <EVT/io/manager.hpp>

namespace IO = EVT::core::IO;

class ADCMux {
    private:
        //Pin States
        EVT::core::IO::GPIO::State low = EVT::core::IO::GPIO::State::LOW;
        EVT::core::IO::GPIO::State high = EVT::core::IO::GPIO::State::HIGH;

        //Control pins
        IO::GPIO& ctrlGPIO1;
        IO::GPIO& ctrlGPIO2;

        IO::ADC& adc;

    public:
        ADCMux(EVT::core::IO::GPIO& ctrlGPIO1, EVT::core::IO::GPIO& ctrlGPIO2, IO::ADC& adc) : ctrlGPIO1(ctrlGPIO1), ctrlGPIO2(ctrlGPIO2), adc(adc) {
        }

        uint32_t getInput() {
            return adc.readRaw();
        }

        //Set the output pins based on an input number
        void setOutputPin(uint8_t number) {
            if((number & 0x1) == 0 && (number & 0x2) == 0) {
                ctrlGPIO1.writePin(low);
                ctrlGPIO2.writePin(low);
            } else if((number & 0x1) == 0 && (number & 0x2) == 0x2) {
                ctrlGPIO1.writePin(high);
                ctrlGPIO2.writePin(low);
            } else if((number & 0x1) == 1 && (number & 0x2) == 0) {
                ctrlGPIO1.writePin(low);
                ctrlGPIO2.writePin(high);
            } else if((number & 0x1) == 1 && (number & 0x2) == 0x2) {
                ctrlGPIO1.writePin(high);
                ctrlGPIO2.writePin(high);
            }
        }
};