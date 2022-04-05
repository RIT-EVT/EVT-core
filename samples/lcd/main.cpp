#include <EVT/dev/LCD.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace DEV = EVT::core::DEV;
namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // creates GPIO pins for LCD
    IO::GPIO&regSelect = IO::getGPIO<IO::Pin::LCD>();
    IO::GPIO& reset = IO::getGPIO<IO::Pin::LCD>();
    IO::GPIO& chipSelect = IO::getGPIO<IO::Pin::LCD>();
    
    // Setup SPI 
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, EVT::core::IO::Pin::SPI_MOSI, EVT::core::IO::Pin::SPI_MISO>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPI_MODE3, SPI_MSB_FIRST);

    // Sets up LCD
    DEV::LCD lcd(regSelect, reset, chipSelect, spi, bitMap)


    while (true) {
        uart.printf("LCD Status: %d \n", );
        time::wait(100);
    }

    return 0;
}
