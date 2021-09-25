/**
 * This sample will demo the basic functionality for the timer driver
 */
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

IO::GPIO* ledGPIO;
IO::GPIO* interruptGPIO2Hz;
IO::GPIO* interruptGPIOStopStart;
IO::GPIO* reloadGPIO;

void timer2IRQHandler(TIM_HandleTypeDef *htim) {
    IO::GPIO::State state = ledGPIO->readPin();
    IO::GPIO::State toggleState = state == IO::GPIO::State::HIGH ? IO::GPIO::State::LOW : IO::GPIO::State::HIGH;
    ledGPIO->writePin(toggleState);
    interruptGPIO2Hz->writePin(toggleState);
}

void timer15IRQHandler(TIM_HandleTypeDef *htim) {
    IO::GPIO::State state = interruptGPIOStopStart->readPin();
    IO::GPIO::State toggleState = state == IO::GPIO::State::HIGH ? IO::GPIO::State::LOW : IO::GPIO::State::HIGH;
    interruptGPIOStopStart->writePin(toggleState);
}

void timer16IRQHandler(TIM_HandleTypeDef *htim) {
    IO::GPIO::State state = reloadGPIO->readPin();
    IO::GPIO::State toggleState = state == IO::GPIO::State::HIGH ? IO::GPIO::State::LOW : IO::GPIO::State::HIGH;
    reloadGPIO->writePin(toggleState);
}

int main() {
    // Initialize system
    IO::init();

    // Setup GPIO
    ledGPIO = &IO::getGPIO<IO::Pin::LED>();
    interruptGPIO2Hz = &IO::getGPIO<IO::Pin::PC_3>(IO::GPIO::Direction::OUTPUT);
    interruptGPIOStopStart = &IO::getGPIO<IO::Pin::PC_2>(IO::GPIO::Direction::OUTPUT);
    reloadGPIO = &IO::getGPIO<IO::Pin::PC_0>(IO::GPIO::Direction::OUTPUT);


    // Setup the Timer
    auto timer2 = DEV::Timerf302x8(TIM2, 500, timer2IRQHandler);
    timer2.reloadTimer();  // Added so cpplint does not complain about unused variable
    auto timer15 = DEV::Timerf302x8(TIM15, 100, timer15IRQHandler);
    auto timer16 = DEV::Timerf302x8(TIM16, 200, timer16IRQHandler);

    while (1) {
        EVT::core::time::wait(500);
        timer15.stopTimer();
        timer16.reloadTimer();
        EVT::core::time::wait(500);
        timer15.startTimer();
    }
}
