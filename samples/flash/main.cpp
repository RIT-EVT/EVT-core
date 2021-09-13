/**
 * This example will show off reading and writing to flash. This will read
 * from memory (a 8, 16, 32, and 64 bit value). Print out the result, then
 * write out to memory the same values.
 *
 * Unlike other tests this will only happen once as the point is to see
 * if the values persist after a reset.
 */
#include <EVT/io/ADC.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    IO::Flash& flash = IO::getFlash();

    uart.printf("\r\nStarting Flash test\r\n");

    time::wait(500);
    
    uart.printf("\r\nReading from Flash\r\n");
    
    uint8_t byteValue = flash.readByte(0);
    uint16_t halfWordValue = flash.readHalfWord(16);
    uint32_t wordValue = flash.readWord(32);
    uint64_t doubleValue = flash.readDoubleWord(64);

    uart.printf("Byte value read from flash: %x\r\n", byteValue);
    uart.printf("Half word value: %x\r\n", halfWordValue);
    uart.printf("Word value: %x\r\n", wordValue);
    uart.printf("Double value: %x\r\n", doubleValue);

    uart.printf("\r\nWriting to Flash\r\n");
    
    flash.writeByte(0, 0xAA);
    flash.writeHalfWord(16, 0xAABB);
    flash.writeWord(32, 0xAABBCCDD);
    flash.writeDoubleWord(64, 0xAABBCCDDEE);

    uart.printf("\r\nRe-reading from Flash\r\n");

    byteValue = flash.readByte(0);
    halfWordValue = flash.readHalfWord(16);
    wordValue = flash.readWord(32);
    doubleValue = flash.readDoubleWord(64);

    uart.printf("Byte value read from flash: %x\r\n", byteValue);
    uart.printf("Half word value: %x\r\n", halfWordValue);
    uart.printf("Word value: %x\r\n", wordValue);
    uart.printf("Double value: %x\r\n", doubleValue);
}
