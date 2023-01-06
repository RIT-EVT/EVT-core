
#include <EVT/io/manager.hpp>

#include "EVT/dev/RTCTimer.hpp"
#include "EVT/dev/platform/f3xx/RTCf3xx.hpp"

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

int main() {
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    //Specific implementation of DEV::RTC, clock type can vary
    DEV::RTCf3xx clock;

    DEV::RTCTimer timer(clock, 5000);

    uart.printf("\r\nTimer starting!\r\n");
    uart.printf("Current time: %d\r\n", timer.getTime());
    timer.startTimer();
    uart.printf("Waiting...\r\n");
    time::wait(1000);
    uart.printf("Stopped...\r\n");
    timer.stopTimer();
    uart.printf("Current time: %d\r\n", timer.getTime());// should be same
    uart.printf("Waiting...\r\n");
    time::wait(1000);
    uart.printf("Current time: %d\r\n", timer.getTime());// should be same
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