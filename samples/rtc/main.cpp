/**
 * This example prints out the time as read in from the RTC
 * every second.
 */
#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    EVT::core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting RTC test\r\n");

    // Start RTC and set the default time
    time::TimeStamp time;
    time.year = 24;
    time.month = 9;
    time.day = 10;

    time.hour = 13;
    time.minute = 42;
    time.second = 35;

    DEV::RTC& rtc = DEV::getRTC();
    rtc.setTime(time);

    time::wait(500);

    uint32_t epochTime;

    while (1) {
        rtc.getTime(time);

        uart.printf("%d/%d/%d %d:%d:%d\r\n",
                    time.day, time.month, time.year,
                    time.hour, time.minute, time.second);

        epochTime = rtc.getTime();
        uart.printf("Timestamp since Epoch: %d\r\n\r\n", epochTime);
        time::wait(1000);
    }
}
