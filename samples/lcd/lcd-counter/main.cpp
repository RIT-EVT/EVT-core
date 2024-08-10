/**
 * Sample code for displaying a counting number on the LCD display.
 */

#include <core/dev/LCD.hpp>
#include <core/dev/LED.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <cstdio>

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

    uint8_t number = 0;

    while (true) {
        lcd.clearArea(64, 1, 7, 0);
        char buffer[128] = {};
        snprintf(buffer, (8), "%d", (number));

        lcd.writeText(buffer, 7, 0, core::DEV::LCD::SMALL, true);

        number++;
        time::wait(500);
    }

    return 0;
}
