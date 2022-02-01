
#include <EVT/io/manager.hpp>

#include "EVT/dev/RTCTimer.hpp"
#include <EVT/dev/platform/f3xx/f302x8/RTC302x8.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

int main() {
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    //Specific implementation of DEV::RTC, clock type can vary
    DEV::RTCf302x8 clock;

    DEV::RTCTimer timer(clock, 5000);

    uart.printf("\nTimer starting!\n");
    uart.printf("Current time: %d\n", timer.getTime());
    timer.startTimer();
    uart.printf("Waiting...\n");
    time::wait(1000);
    uart.printf("Stopped...\n");
    timer.stopTimer();
    uart.printf("Current time: %d\n", timer.getTime());// should be same
    uart.printf("Waiting...\n");
    time::wait(1000);
    uart.printf("Current time: %d\n", timer.getTime());// should be same
    timer.startTimer();

    while (1) {
        if (timer.hasGoneOff()) {
            uart.printf("Gone off!\n");
            uart.printf("Current time: %d\n", timer.getTime());
            timer.reloadTimer();
            uart.printf("\nTimer starting!\n");
            timer.startTimer();
        }
    }
}