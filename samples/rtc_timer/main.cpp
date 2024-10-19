
#include <core/manager.hpp>

#include <core/dev/RTCTimer.hpp>

namespace io  = core::io;
namespace dev = core::dev;

int main() {
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // Specific implementation of dev::RTC, clock type can vary
    dev::RTC& clock = dev::getRTC();

    dev::RTCTimer timer(clock, 5000);

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