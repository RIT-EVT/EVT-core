/**
 * Example of PWM input.
 * This sample will measure the duty cycle, frequency, and period of a PWM signal.
 */

#include <core/manager.hpp>
#include <core/io/PWM_INPUT.hpp>
#include <core/io/UART.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

int main(void) {
    // Initialize system
    core::platform::init();

    // Setup UART
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    uart.printf("Starting PWM input capture\n\r");

    uint32_t Period    = 0; // ICValue
    uint32_t Frequency = 0;
    uint8_t DutyCycle = 0;

    io::PWM_INPUT& pwmInput = io::getPWM_INPUT<io::Pin::PB_15>();

    while (1) {
        Period    = pwmInput.getPeriod();
        Frequency = pwmInput.getFrequency();
        DutyCycle = pwmInput.getDutyCycle();
        uart.printf("\n\rPeriod: %d\n\r", Period);
        uart.printf("\n\rFrequency: %d\n\r", Frequency);
        uart.printf("\n\rDuty: %d\n\r", DutyCycle);
        uart.printf("\n\r----------------------------------\n\r");
        time::wait(1000);
    }
}
