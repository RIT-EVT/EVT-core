#ifndef _EVT_UARTf302x8_
#define _EVT_UARTf302x8_

#include <stdint.h>

#include <EVT/io/UART.hpp>

#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class UARTf302x8 : public UART {
public:
    /**
     * Create an instance of the STMF302x8 UART interface using the provided
     * TX and RX pins.
     *
     * @param txPin The UART TX pin.
     * @param rxPin THe UART RX pin.
     * @param baudrate The baudrate to operate the UART with/
     */
    UARTf302x8(Pin txPin, Pin rxPin, uint32_t baudrate, uint8_t portID);

    /**
     * Set the baudrate that the UART will operate with.
     *
     * @param baudrate The new baudrate to use
     */
    void setBaudrate(uint32_t baudrate);

    /**
     * Set the format that data will be communicated with over UART.
     *
     * DEFAULT: 8 bit words, no parity, 1 stop bit
     *
     * @param wordLength The number of bits in a work (5-8)
     * @param parity The parity settings to use.
     * @param numStopBits The number of stop bits (1-2)
     */
    virtual void setFormat(WordLength wordLength=WordLength::EIGHT,
            Parity parity=Parity::NONE,
            NumStopBits numStopBits=NumStopBits::ONE) = 0;

    /**
     * Sends a serial break condition over UART.
     */
     void sendBreak();

    /**
     * Determins if the UART is currently readable.
     *
     * @return True if the UART is readable.
     */
     bool isReadable();

    /**
     * Determins if the UART is currently writable.
     *
     * @return True if the UART is writable
     */
     bool isWritable();

    /**
     * Put a single character to the UART module.
     *
     * @param c The character to send over UART.
     */
     void putc(char c);

    /**
     * Put a null-terminal string out over UART.
     *
     * @param s The null-terminated string to put over UART.
     */
     void puts(const char* s);

    /**
     * Blocking call to read a single character from the UART module.
     *
     * @return The character read in over UART.
     */
     char getc();

    /**
     * Print a formatted string over UART, not great performance so best in
     * test situations not production.
     *
     * @param format The format string to print out.
     */
     void printf(const char* format, ...);

    /**
     * Write out a single byte over UART.
     *
     * @param byte The byte to write out over UART.
     */
     void write(uint8_t byte);

    /**
     * Blocking reading of a single byte from the UART.
     *
     * @return The byte read in from UART.
     */
     uint8_t read();

    /**
     * Write an arbitrary number of bytes out over UART.
     *
     * @param bytes The data to send out over UART
     * @param size The number of bytes to send out over UART.
     */
     void writeBytes(uint8_t* bytes, size_t size);

    /**
     * Blocking reading of an arbitrary number of bytes in over UART.
     *
     * @param bytes The data buffer to fill
     * @param size The number of bytes to read in.
     */
     void readBytes(uint8_t* bytes, size_t size);

private:
     /// HAL representation of the UART
     UART_HandleTypeDef halUART;
};
}  // namespace EVT::core::IO
#endif
