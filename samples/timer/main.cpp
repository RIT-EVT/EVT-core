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
IO::GPIO* interruptGPIO;

void timerIRQHandler(TIM_HandleTypeDef *htim) {
    IO::GPIO::State state = ledGPIO->readPin();
    IO::GPIO::State toggleState = state == IO::GPIO::State::HIGH ? IO::GPIO::State::LOW : IO::GPIO::State::HIGH;
    ledGPIO->writePin(toggleState);
    interruptGPIO->writePin(toggleState);
}

int main() {
    // Initialize system
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Setup GPIO
    ledGPIO = &IO::getGPIO<IO::Pin::LED>();
    interruptGPIO = &IO::getGPIO<IO::Pin::PC_3>(IO::GPIO::Direction::OUTPUT);

    // Setup the Timer
    auto timer = DEV::Timerf302x8(500, timerIRQHandler);

    while (1) {
        uart.printf("Still Running...\n\r");
        EVT::core::time::wait(1000);
    }
}
