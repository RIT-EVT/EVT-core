#include <EVT/io/GPIO.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/dev/ADCMux.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    IO::init();

    IO::GPIO& ctrlGPIO1 = IO::getGPIO<IO::Pin::PB_10>(IO::GPIO::Direction::OUTPUT);
    IO::GPIO& ctrlGPIO2 = IO::getGPIO<IO::Pin::PB_4>(IO::GPIO::Direction::OUTPUT);

    IO::ADC& adc = IO::getADC<IO::Pin::PB_8>();

    DEV::ADCMux mux(ctrlGPIO1, ctrlGPIO2, adc);

    while(1) {
        //Checking if input is valid
        if(mux.getInput() == 0 || mux.getInput() == 1 || mux.getInput() == 2 || mux.getInput() == 3) {
            mux.setOutputPin(mux.getInput());
        }
    }
}

