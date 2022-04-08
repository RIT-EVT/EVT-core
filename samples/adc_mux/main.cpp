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

    IO::GPIO& ctrlGPIO3 = IO::getGPIO<IO::Pin::PB_8>(IO::GPIO::Direction::OUTPUT);

    while(1) {
        ctrlGPIO3.writePin(EVT::core::IO::GPIO::State::LOW);
        mux.setOutputPin(mux.getInput());
        time::wait(500);
        ctrlGPIO3.writePin(EVT::core::IO::GPIO::State::HIGH);
        mux.setOutputPin(mux.getInput());
        time::wait(500);
    }
}

