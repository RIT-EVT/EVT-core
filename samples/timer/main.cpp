/**
 * This sample will demo the basic functionality for the timer driver
 */
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

IO::GPIO* ledGPIO;
IO::GPIO* interruptGPIO2Hz;
IO::GPIO* interruptGPIOStopStart;
IO::GPIO* reloadGPIO;

void timer2IRQHandler(void* htim) {
    IO::GPIO::State state = ledGPIO->readPin();
    IO::GPIO::State toggleState = state == IO::GPIO::State::HIGH ? IO::GPIO::State::LOW : IO::GPIO::State::HIGH;
    ledGPIO->writePin(toggleState);
    interruptGPIO2Hz->writePin(toggleState);
}

void timer15IRQHandler(void* htim) {
    IO::GPIO::State state = interruptGPIOStopStart->readPin();
    IO::GPIO::State toggleState = state == IO::GPIO::State::HIGH ? IO::GPIO::State::LOW : IO::GPIO::State::HIGH;
    interruptGPIOStopStart->writePin(toggleState);
}

void timer16IRQHandler(void* htim) {
    IO::GPIO::State state = reloadGPIO->readPin();
    IO::GPIO::State toggleState = state == IO::GPIO::State::HIGH ? IO::GPIO::State::LOW : IO::GPIO::State::HIGH;
    reloadGPIO->writePin(toggleState);
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup GPIO
    ledGPIO = &IO::getGPIO<IO::Pin::LED>();
    interruptGPIO2Hz = &IO::getGPIO<IO::Pin::PC_3>(IO::GPIO::Direction::OUTPUT);
    interruptGPIOStopStart = &IO::getGPIO<IO::Pin::PC_2>(IO::GPIO::Direction::OUTPUT);
    reloadGPIO = &IO::getGPIO<IO::Pin::PC_0>(IO::GPIO::Direction::OUTPUT);

    // Setup the Timer
    DEV::Timer& timer2 = DEV::getTimer<DEV::MCUTimer::Timer2>(500);
    DEV::Timer& timer15 = DEV::getTimer<DEV::MCUTimer::Timer15>(100);
    DEV::Timer& timer16 = DEV::getTimer<DEV::MCUTimer::Timer16>(200);

    timer2.startTimer(timer2IRQHandler);
    timer15.startTimer(timer15IRQHandler);
    timer16.startTimer(timer16IRQHandler);

    while (1) {
        EVT::core::time::wait(500);
        timer15.stopTimer();
        timer16.reloadTimer();
        EVT::core::time::wait(500);
        timer15.startTimer();
    }
}
