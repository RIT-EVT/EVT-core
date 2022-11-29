//
// Created by Zachary Lineman on 11/14/22.
//

#include "EVT/dev/LCD.hpp"
#include "EVT/dev/LED.hpp"
#include "EVT/io/UART.hpp"
#include "EVT/io/manager.hpp"
#include "EVT/utils/time.hpp"
#include <string>

/**
 * Sample code for displaying EVT logo onto an LCD display
 */

namespace DEV = EVT::core::DEV;
namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_500KHZ;

constexpr uint8_t deviceCount = 1;

IO::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    IO::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::PB_10, IO::Pin::PB_11>(9600);

    // Uses HUDL 1.0 Pins
    IO::GPIO& regSelect = IO::getGPIO<IO::Pin::PA_3>(EVT::core::IO::GPIO::Direction::OUTPUT);
    IO::GPIO& reset = IO::getGPIO<IO::Pin::PB_3>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0] = &IO::getGPIO<IO::Pin::PB_12>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);

    // Setup SPI
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, IO::Pin::SPI_MOSI>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPI_MODE0, SPI_MSB_FIRST);

    // Sets up LCD
    uart.printf("Creating LCD Object...\n\r");
    EVT::core::DEV::LCD lcd(regSelect, reset, spi);
    uart.printf("Initializing LCD...\n\r");
    lcd.initLCD();
    lcd.clearLCD();

    uint8_t number = 0;

    while (true) {
        lcd.clearArea(64, 8, 7, 0);
        const char* dst = std::to_string(number).c_str();
        lcd.writeText(dst, 7, 0, true);

        number ++;
        time::wait(500);
    }

    return 0;
}
