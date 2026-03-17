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
io::GPIO* interruptGPIO2Hz;
io::GPIO* interruptGPIOStopStart;
io::GPIO* reloadGPIO;

void timer2IRQHandler(void* context, void* htim) {
    const io::GPIO::State state       = ledGPIO->readPin();
    const io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
    ledGPIO->writePin(toggleState);
    interruptGPIO2Hz->writePin(toggleState);

    auto* uart = static_cast<io::UART*>(context);

    uart->printf("Timer 2 Interrupt\n\r");
}

void timer15IRQHandler(void* context, void* htim) {
    const io::GPIO::State state       = interruptGPIOStopStart->readPin();
    const io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
    interruptGPIOStopStart->writePin(toggleState);
}

void timer16IRQHandler(void* context, void* htim) {
    const io::GPIO::State state       = reloadGPIO->readPin();
    const io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
    reloadGPIO->writePin(toggleState);
}

int main() {
    // Initialize system
    core::platform::init();

    // Setup GPIO
    ledGPIO                = &io::getGPIO<io::Pin::LED>();
    interruptGPIO2Hz       = &io::getGPIO<io::Pin::PC_3>(io::GPIO::Direction::OUTPUT);
    interruptGPIOStopStart = &io::getGPIO<io::Pin::PC_2>(io::GPIO::Direction::OUTPUT);
    reloadGPIO             = &io::getGPIO<io::Pin::PC_0>(io::GPIO::Direction::OUTPUT);

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // Set up the logger with a UART, logLevel, and clock
    // If timestamps aren't needed, don't set the logger's clock
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::INFO);
    dev::RTC& rtc = dev::getRTC();
    log::LOGGER.setClock(&rtc);

    // Set up the Timer
    dev::Timer& sampleTimer1 = dev::getTimer<dev::MCUTimer::Timer2>(1000);

#ifdef STM32F4xx
    // F4xx does not support Timers 15 & 16, change them to Timer11 & Timer12
    dev::Timer& sampleTimer2 = dev::getTimer<dev::MCUTimer::Timer11>(200);
    dev::Timer& sampleTimer3 = dev::getTimer<dev::MCUTimer::Timer12>(200);
#else
    dev::Timer& sampleTimer2 = dev::getTimer<dev::MCUTimer::Timer15>(1000);
    dev::Timer& sampleTimer3 = dev::getTimer<dev::MCUTimer::Timer16>(1000);
#endif

    // If you need access to a class or structure you defined in main() within your timer, you can use the context of
    // an IRQ handler. This will take a reference to anything you pass and make it available to your function in the
    // context parameter.
    sampleTimer1.startTimer(timer2IRQHandler, &uart);

    // Using nullptr here since we do not need a context for the sample.
    sampleTimer2.startTimer(timer15IRQHandler, nullptr);
    sampleTimer3.startTimer(timer16IRQHandler, nullptr);

    while (1) {
        core::time::wait(500);
        sampleTimer2.stopTimer();
        sampleTimer3.reloadTimer();
        core::time::wait(500);
        sampleTimer2.startTimer();
    }
}
