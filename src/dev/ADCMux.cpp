#include <EVT/io/ADC.hpp>
#include <EVT/dev/ADCMux.hpp>
#include <EVT/io/pin.hpp>

namespace EVT::core::DEV {

    ADCMux::ADCMux(EVT::core::IO::GPIO& ctrlGPIO1, EVT::core::IO::GPIO& ctrlGPIO2, IO::ADC& adc) : ctrlGPIO1(ctrlGPIO1), ctrlGPIO2(ctrlGPIO2), adc(adc) {
    }

    uint32_t ADCMux::getInput() {
        return adc.readRaw();
    }

    //Set the output pins based on an input number
    void ADCMux::setOutputPin(uint8_t number) {
        switch(number) {
            case 0:
                ctrlGPIO1.writePin(low);
                ctrlGPIO2.writePin(low);
            case 1:
                ctrlGPIO1.writePin(high);
                ctrlGPIO2.writePin(low);
            case 2:
                ctrlGPIO1.writePin(low);
                ctrlGPIO2.writePin(high);
            case 3:
                ctrlGPIO1.writePin(high);
                ctrlGPIO2.writePin(high);
        }
    }
}


