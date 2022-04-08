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
}


