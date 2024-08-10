/**
 * Sample code for displaying a moving ball animation on an LCD.
 */

#include <core/dev/LCD.hpp>
#include <core/dev/LED.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace DEV  = core::DEV;
namespace IO   = core::IO;
namespace time = core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_500KHZ;

constexpr uint8_t deviceCount = 1;

IO::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Uses HUDL 1.0 Pins
    IO::GPIO& regSelect = IO::getGPIO<IO::Pin::PA_3>(core::IO::GPIO::Direction::OUTPUT);
    IO::GPIO& reset     = IO::getGPIO<IO::Pin::PB_3>(core::IO::GPIO::Direction::OUTPUT);

    devices[0] = &IO::getGPIO<IO::Pin::PB_12>(core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);

    // Setup SPI
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, IO::Pin::SPI_MOSI>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, IO::SPI::SPIMode::SPI_MODE0, SPI_MSB_FIRST);

    // Sets up LCD
    uart.printf("Creating LCD Object...\n\r");
    core::DEV::LCD lcd(regSelect, reset, spi);
    uart.printf("Initializing LCD...\n\r");
    lcd.initLCD();
    lcd.clearLCD();

    uint8_t ball[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t col     = 0;
    uint8_t page    = 0;

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
