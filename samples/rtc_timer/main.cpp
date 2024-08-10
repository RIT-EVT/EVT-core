
#include <core/manager.hpp>

#include <core/dev/RTCTimer.hpp>

namespace IO  = core::IO;
namespace DEV = core::DEV;

int main() {
    core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Specific implementation of DEV::RTC, clock type can vary
    DEV::RTC& clock = DEV::getRTC();

    DEV::RTCTimer timer(clock, 5000);

    uart.printf("\r\nTimer starting!\r\n");
    uart.printf("Current time: %d\r\n", timer.getTime());
    timer.startTimer();
    uart.printf("Waiting...\r\n");
    time::wait(1000);
    uart.printf("Stopped...\r\n");
    timer.stopTimer();
    uart.printf("Current time: %d\r\n", timer.getTime()); // should be same
    uart.printf("Waiting...\r\n");
    time::wait(1000);
    uart.printf("Current time: %d\r\n", timer.getTime()); // should be same
    timer.startTimer();

    while (1) {
        if (timer.hasGoneOff()) {
            uart.printf("Gone off!\r\n");
            uart.printf("Current time: %d\r\n", timer.getTime());
            timer.reloadTimer();
            uart.printf("\r\nTimer starting!\r\n");
            timer.startTimer();
        }
    }
}