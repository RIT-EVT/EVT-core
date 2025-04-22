/**
 * This sample will demo the basic functionality for the timer driver
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io  = core::io;
namespace dev = core::dev;
namespace log = core::log;

io::GPIO* ledGPIO;
// io::GPIO* interruptGPIO2Hz;
// io::GPIO* interruptGPIOStopStart;
// io::GPIO* reloadGPIO;

void timerIRQHandler(void* htim) {
    io::GPIO::State state       = ledGPIO->readPin();
    io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
    ledGPIO->writePin(toggleState);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Flash LED");
}

// void timer15IRQHandler(void* htim) {
//     io::GPIO::State state       = interruptGPIOStopStart->readPin();
//     io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
//     interruptGPIOStopStart->writePin(toggleState);
// }
//
// void timer16IRQHandler(void* htim) {
//     io::GPIO::State state       = reloadGPIO->readPin();
//     io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
//     reloadGPIO->writePin(toggleState);
// }

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // Print directly to the UART to show that the file has been run despite any
    // problems with the Logger

    // Set up the logger with a UART, logLevel, and clock
    // If timestamps aren't needed, don't set the logger's clock
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    // Setup GPIO
    ledGPIO                = &io::getGPIO<io::Pin::LED>();
    // interruptGPIO2Hz       = &io::getGPIO<io::Pin::PC_3>(io::GPIO::Direction::OUTPUT);
    // interruptGPIOStopStart = &io::getGPIO<io::Pin::PC_2>(io::GPIO::Direction::OUTPUT);
    // reloadGPIO             = &io::getGPIO<io::Pin::PC_0>(io::GPIO::Direction::OUTPUT);

    // Setup the Timer
    dev::TimerConfiguration configuration = {
        TIM_COUNTERMODE_UP,
        TIM_CLOCKDIVISION_DIV1,
        TIM_AUTORELOAD_PRELOAD_ENABLE,
        TIM_CLOCKSOURCE_INTERNAL,
        TIM_TRGO_RESET,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    // F4xx does not support Timers 15 & 16, change them to Timer11 & Timer12
    // dev::Timer& timer = dev::getTimer<dev::MCUTimer::Timer1>(1000, configuration);
    // dev::Timer& timer16 = dev::getTimer<dev::MCUTimer::Timer16>(200);

    timer.startTimer(timerIRQHandler);
    // timer2.startTimer(timer2IRQHandler);
    // timer15.startTimer(timer15IRQHandler);
    // timer16.startTimer(timer16IRQHandler);

    while (1) {

    }
}
