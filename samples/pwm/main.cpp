/**
 * This example shows off a basic PWM signal. You will need to use a logic
 * analyzer to see the generated square wave.
 */
#include <core/io/PWM.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    core::log::LOGGER.setUART(&uart);
    core::log::LOGGER.setLogLevel(core::log::Logger::LogLevel::DEBUG);

    io::PWM& pwm = io::getPWM<io::Pin::PA_8>();
    // 1000000 microseconds (1 second) period
    pwm.setPeriod(1000000 / 5);
    // 50 % duty cycle
    pwm.setDutyCycle(50);

    while (1) {
        core::time::wait(1000);
        // pwm.setDutyCycle(70);
        // core::time::wait(5000);
        // pwm.setDutyCycle(30);
        // core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "Hit");
    }
}