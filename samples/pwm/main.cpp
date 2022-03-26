/**
 * This example shows off a basic PWM signal. You will need to use a logic
 * analyzer to see the generated square wave.
 */
#include <EVT/io/PWM.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    IO::PWM& pwm = IO::getPWM<IO::Pin::PC_0>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    // 1000000 microseconds (1 second) period
    pwm.setPeriod(100000);
    // 50 % duty cycle
    pwm.setDutyCycle(50);

    uint32_t autoReload = 0;
    uint32_t prescaler = -1;
    uint32_t clockFrequency = HAL_RCC_GetSysClockFreq();

    // Required loop in order to determine a prescaler which will bring the
    // autoreload value into a valid range.
    do {
        prescaler++;
        autoReload = (uint32_t) (((uint64_t) (clockFrequency / (prescaler + 1))) * 1000000 / pwm.getPeriod());
    } while (autoReload > 65535);
    uart.printf("p1: %d\n\r", prescaler);
    uart.printf("a1: %d\n\r", autoReload);

    prescaler = -1;
    clockFrequency = HAL_RCC_GetSysClockFreq();

    // Required loop in order to determine a prescaler which will bring the
    // autoreload value into a valid range.
    do {
        prescaler++;
        autoReload = clockFrequency / (prescaler + 1) / (pwm.getPeriod() / (double) 1000000);
    } while (autoReload > 65535);
    uart.printf("p2: %d\n\r", prescaler);
    uart.printf("a2: %d\n\r", autoReload);

    while (1) {
        time::wait(10);
    }
}
