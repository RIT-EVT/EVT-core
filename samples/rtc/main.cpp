/**
 * This example prints out the time as read in from the RTC
 * every second.
 */
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/dev/platform/f3xx/f302x8/RTC302x8.hpp>
#include <EVT/io/UART.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting RTC test\r\n");

    // Start RTC and set the default time
    time::TimeStamp time;
    time.year = 2021;
    time.month = 9;
    time.day = 10;

    time.hour = 13;
    time.minute = 42;
    time.second = 35;

    DEV::RTCf302x8 rtc;
    rtc.setTime(time);

    time::wait(500);


    while(1) {
        rtc.getTime(time);
        
        uart.printf("%d/%d/%d %d:%d:%d\r\n",
                time.day, time.month, time.year,
                time.hour, time.minute, time.second);
        time::wait(1000);
    }

}
