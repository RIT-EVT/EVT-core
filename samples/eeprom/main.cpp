/**
 * Example that handles data reading and writing to an EEPROM.
 *
 * The sample will read and write two bytes to the EEPROM which
 * should contain the values "aa" and "bb".
 */
#include <stdint.h>

#include <EVT/dev/storage/platform/M24C32.hpp>
#include <EVT/io/manager.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

/**
 * The address of the EEPROM listening for reads/writes
 * Set up for M24C23 with all enable values set to 0
 * Docs can be found here: https://www.st.com/content/ccc/resource/technical/document/datasheet/80/4e/8c/54/f2/63/4c/4a/CD00001012.pdf/files/CD00001012.pdf/jcr:content/translations/en.CD00001012.pdf
 */
constexpr uint8_t I2C_SLAVE_ADDR = 0x50;

constexpr uint8_t BYTE_ADDRESS = 0x00;
constexpr uint8_t HALF_WORD_ADDRESS = 0x01;
constexpr uint8_t WORD_ADDRESS = 0x03;
constexpr uint8_t BYTE_ARR_ADDRESS = 0x07;
constexpr uint8_t HALF_WORD_ARR_ADDRESS = 0x09;
constexpr uint8_t WORD_ARR_ADDRESS = 0x13;

constexpr uint8_t BYTE_DATA = 0xaa;
constexpr uint16_t HALF_WORD_DATA = 0xabcd;
constexpr uint32_t WORD_DATA = 0xfedcba98;
constexpr uint8_t BYTE_ARR_LENGTH = 2;
uint8_t BYTE_ARR_DATA[BYTE_ARR_LENGTH] = {0xab, 0xba};
constexpr uint8_t HALF_WORD_ARR_LENGTH = 2;
uint16_t HALF_WORD_ARR_DATA[HALF_WORD_ARR_LENGTH] = {0xbbbb, 0xcccc};
constexpr uint8_t WORD_ARR_LENGTH = 2;
uint32_t WORD_ARR_DATA[WORD_ARR_LENGTH] = {0x01234567, 0x89abcdef};

int main() {
    // Initialize system
    IO::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    DEV::M24C32 eeprom = DEV::M24C32(I2C_SLAVE_ADDR, i2c);

    uart.printf("Starting EEPROM test\n\r");

    eeprom.writeByte(BYTE_ADDRESS, BYTE_DATA);
    eeprom.writeHalfWord(HALF_WORD_ADDRESS, HALF_WORD_DATA);
    eeprom.writeWord(WORD_ADDRESS, WORD_DATA);

    eeprom.writeBytes(BYTE_ARR_ADDRESS, BYTE_ARR_DATA, BYTE_ARR_LENGTH);
    eeprom.writeHalfWords(HALF_WORD_ARR_ADDRESS, HALF_WORD_ARR_DATA, HALF_WORD_ARR_LENGTH);
    eeprom.writeWords(WORD_ARR_ADDRESS, WORD_ARR_DATA, WORD_ARR_LENGTH);

    uart.printf("Byte Read: %#x\n\r", eeprom.readByte(BYTE_ADDRESS));
    uart.printf("Half Word Read: %#x\n\r", eeprom.readHalfWord(HALF_WORD_ADDRESS));
    uart.printf("Word Read: %#x\n\r", eeprom.readWord(WORD_ADDRESS));

    uint8_t byteBuf[BYTE_ARR_LENGTH];
    eeprom.readBytes(BYTE_ARR_ADDRESS, byteBuf, BYTE_ARR_LENGTH);
    for (uint8_t i : byteBuf) {
        uart.printf("Byte Read: %#x\n\r", i);
    }

    uint16_t hWordBuf[HALF_WORD_ARR_LENGTH];
    eeprom.readHalfWords(HALF_WORD_ARR_ADDRESS, hWordBuf, HALF_WORD_ARR_LENGTH);
    for (uint16_t i : hWordBuf) {
        uart.printf("Half Word Read: %#x\n\r", i);
    }

    uint32_t wordBuf[WORD_ARR_LENGTH];
    eeprom.readWords(WORD_ARR_ADDRESS, wordBuf, WORD_ARR_LENGTH);
    for (uint32_t i : wordBuf) {
        uart.printf("Word Read: %#x\n\r", i);
    }
}
