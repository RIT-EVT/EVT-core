#ifndef _EVT_UARTf3xx_
#define _EVT_UARTf3xx_

#include <stdint.h>

#include <EVT/io/UART.hpp>

#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class UARTf3xx : public UART {
public:
    /**
     * Create an instance of the STMF302x8 UART interface using the provided
     * TX and RX pins.
     *
     * @param txPin[in] The UART TX pin.
     * @param rxPin[in] THe UART RX pin.
     * @param baudrate[in] The baudrate to operate the UART with/
     */
    UARTf3xx(Pin txPin, Pin rxPin, uint32_t baudrate);

    /**
     * Set the baudrate that the UART will operate with.
     *
     * @param baudrate[in] The new baudrate to use
     */
    void setBaudrate(uint32_t baudrate);

    /**
     * Set the format that data will be communicated with over UART.
     *
     * DEFAULT: 8 bit words, no parity, 1 stop bit
     *
     * @param wordLength[in] The number of bits in a work (5-8)
     * @param parity[in] The parity settings to use.
     * @param numStopBits[in] The number of stop bits (1-2)
     */
    void setFormat(WordLength wordLength = WordLength::EIGHT,
                   Parity parity = Parity::NONE,
                   NumStopBits numStopBits = NumStopBits::ONE);

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
     * @param c[in] The character to send over UART.
     */
    void putc(char c);

    /**
     * Put a null-terminal string out over UART.
     *
     * @param s[in] The null-terminated string to put over UART.
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
     * @param format[in] The format string to print out.
     */
    void printf(const char* format, ...);

    /**
     * Write out a single byte over UART.
     *
     * @param byte[in] The byte to write out over UART.
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
     * @param bytes[in] The data to send out over UART
     * @param size[in] The number of bytes to send out over UART.
     */
    void writeBytes(uint8_t* bytes, size_t size);

    /**
     * Blocking reading of an arbitrary number of bytes in over UART.
     *
     * @param bytes[out] The data buffer to fill
     * @param size[in] The number of bytes to read in.
     */
    void readBytes(uint8_t* bytes, size_t size);

private:
    /// HAL representation of the UART
    UART_HandleTypeDef halUART;
};
}// namespace EVT::core::IO
#endif
