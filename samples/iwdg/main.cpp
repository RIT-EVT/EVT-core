#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/dev/platform/f3xx/f302x8/IWDGf302x8.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    IO::init();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    DEV::IWDGf302x8 iwdg = DEV::IWDGf302x8(4095);

    uart.printf("Starting IWDG test...\n\r\n\r");

    for (int i = 0; i < 3; i++) {
        time::wait(400);
        iwdg.refresh();
        uart.printf("IWDG refreshed\n\r");
    }

    while (1);
}