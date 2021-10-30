/**
 * Example that handles data reading and writing to an EEPROM.
 *
 * The sample will read and write two bytes to the EEPROM which
 * should contain the values "aa" and "bb".
 */
#include <stdint.h>

#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/dev/storage/platform/M24C32.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

/**
 * The address of the EEPROM listening for reads/writes
 * Set up for M24C23 with all enable values set to 0
 * Docs can be found here: https://www.st.com/content/ccc/resource/technical/document/datasheet/80/4e/8c/54/f2/63/4c/4a/CD00001012.pdf/files/CD00001012.pdf/jcr:content/translations/en.CD00001012.pdf
 * TODO: Find better way to share the docs
 * */
constexpr uint8_t I2C_SLAVE_ADDR = 0x50;

constexpr uint8_t ADDRESS_0 = 0x00;
constexpr uint8_t ADDRESS_1 = 0x01;
constexpr uint8_t ADDRESS_2 = 0x02;

constexpr uint8_t DATA_0 = 0xaa;
constexpr uint8_t DATA_1 = 0xbb;

int main() {
    // Initialize system
    IO::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    DEV::M24C32 eeprom = DEV::M24C32(I2C_SLAVE_ADDR, i2c);

    uart.printf("Starting EEPROM test\n\r");

    eeprom.writeByte(ADDRESS_0, DATA_0);
    eeprom.writeByte(ADDRESS_1, DATA_1);

    uint16_t longData = DATA_0;
    longData = (longData << 8) + DATA_1;
    eeprom.writeHalfWord(ADDRESS_2, longData);
    uint32_t data = 0xabcdef01;
    eeprom.writeWord(0x04, data);

    uart.printf("Byte Read 0: %#x\n\r", eeprom.readByte(ADDRESS_0));
    uart.printf("Byte Read 1: %#x\n\r", eeprom.readByte(ADDRESS_1));
    uart.printf("Half Word Read: %#x\n\r", eeprom.readHalfWord(ADDRESS_1));
    uart.printf("Word Read: %#x\n\r", eeprom.readWord(0x04));

    uint8_t addresses[3] = {0x00, 0x04, 0x08};
    uint8_t *results = eeprom.readBytes(addresses);
    for (int i = 0; i < sizeof results; ++i) {
        uart.printf("Byte Read: %#x\n\r", results[i]);
    }
}
