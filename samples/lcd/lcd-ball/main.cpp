/**
* Sample code for displaying a moving ball animation on an LCD.
*/

#include <EVT/dev/LCD.hpp>
#include <EVT/dev/LED.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

namespace DEV = EVT::core::DEV;
namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_500KHZ;

constexpr uint8_t deviceCount = 1;

IO::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Uses HUDL 1.0 Pins
    IO::GPIO& regSelect = IO::getGPIO<IO::Pin::PA_3>(EVT::core::IO::GPIO::Direction::OUTPUT);
    IO::GPIO& reset = IO::getGPIO<IO::Pin::PB_3>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0] = &IO::getGPIO<IO::Pin::PB_12>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);

    // Setup SPI
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, IO::Pin::SPI_MOSI>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, IO::SPI::SPIMode::SPI_MODE0, SPI_MSB_FIRST);

    // Sets up LCD
    uart.printf("Creating LCD Object...\n\r");
    EVT::core::DEV::LCD lcd(regSelect, reset, spi);
    uart.printf("Initializing LCD...\n\r");
    lcd.initLCD();
    lcd.clearLCD();

    uint8_t ball[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t col = 0;
    uint8_t page = 0;

    lcd.displayBitMapInArea(ball, 8, 1, page, col);

    while (true) {
        lcd.clearArea(8, 1, page, col);

        col++;
        if (col >= 128) {
            col = 0;
            page++;

            if (page > 7) {
                page = 0;
            }
        }

        lcd.displayBitMapInArea(ball, 8, 1, page, col);

        time::wait(300);
    }

    return 0;
}
