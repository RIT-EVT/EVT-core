/**
 * This sample will demo the basic functionality for the timer driver
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io  = core::io;
namespace dev = core::dev;

io::GPIO* ledGPIO;
io::GPIO* interruptGPIO2Hz;
io::GPIO* interruptGPIOStopStart;
io::GPIO* reloadGPIO;

void timer2IRQHandler(void* htim) {
    io::GPIO::State state       = ledGPIO->readPin();
    io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
    ledGPIO->writePin(toggleState);
    interruptGPIO2Hz->writePin(toggleState);
}

void timer15IRQHandler(void* htim) {
    io::GPIO::State state       = interruptGPIOStopStart->readPin();
    io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
    interruptGPIOStopStart->writePin(toggleState);
}

void timer16IRQHandler(void* htim) {
    io::GPIO::State state       = reloadGPIO->readPin();
    io::GPIO::State toggleState = state == io::GPIO::State::HIGH ? io::GPIO::State::LOW : io::GPIO::State::HIGH;
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

    // Initialize a configuration object for the timer.
    dev::TimerConfiguration configuration = {
        TIM_COUNTERMODE_UP,
        TIM_CLOCKDIVISION_DIV1,
        TIM_AUTORELOAD_PRELOAD_ENABLE,
        TIM_CLOCKSOURCE_INTERNAL,
        TIM_TRGO_RESET,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    // Set up the Timer
    dev::Timer& sampleTimer1 = dev::getTimer<dev::MCUTimer::Timer2>(1000, configuration);

    #ifdef STM32F4xx
    // F4xx does not support Timers 15 & 16, change them to Timer11 & Timer12
    dev::Timer& sampleTimer2 = dev::getTimer<dev::MCUTimer::Timer11>(200, configuration);
    dev::Timer& sampleTimer3 = dev::getTimer<dev::MCUTimer::Timer12>(200, configuration);
    #else
    dev::Timer& sampleTimer2 = dev::getTimer<dev::MCUTimer::Timer15>(1000, configuration);
    dev::Timer& sampleTimer3 = dev::getTimer<dev::MCUTimer::Timer16>(1000, configuration);

    #endif

    sampleTimer1.startTimer(timer2IRQHandler);
    sampleTimer2.startTimer(timer15IRQHandler);
    sampleTimer3.startTimer(timer16IRQHandler);

    while (1) {
        core::time::wait(500);
        sampleTimer2.stopTimer();
        sampleTimer3.reloadTimer();
        core::time::wait(500);
        sampleTimer2.startTimer();
    }
}
