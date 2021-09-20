#ifndef _EVT_UART_
#define _EVT_UART_

#include <cstdint>
#include <cstdlib>

namespace EVT::core::IO {

// Forward declarations:
// The different pins are hardware specific. Forward declarations to allow
// at compilation time the devicision of which pins should be used.
enum class Pin;

/**
 * Interface for UART operations. The UART has the ability for character and
 * byte centered operations.
 *
 * This provides the standard set of features that are shared across all
 * UART modules. Hardware specific features are not included.
 *
 * The UART modules support both character and byte oriented transmit and
 * receive operations. While fundametally these operations are the same,
 * semantically this makes working with character cetered data and byte
 * centered data cleaner.
 *
 * NOTE: You cannot directly make an instance of this class via a constructor.
 * To make an isntance, use the UART::getInstance method.
 */
class UART {
public:

    /**
     * Represents the options for the parity settings that may be used when
     * setting up a UART.
     */
    enum class Parity {
        NONE,
        ODD,
        EVEN,
        MASK,
        SPACE
    };

    /**
     * Represents the possible lengths of words that can exist for UART.
     */
    enum class WordLength {
        FIVE = 5,
        SIX = 6,
        SEVEN = 7,
        EIGHT = 8
    };

    /**
     * Represents the number of stop bits that are used.
     */
    enum class NumStopBits {
        ONE = 1,
        TWO = 2
    };

    /**
     * Creates a new UART instance that will have the given TX and RX pins
     * as well as the given baud rate.
     *
     * @param txPin[in] The UART TX pin.
     * @param rxPin[in] The UART RX pin.
     * @param baudrate[in] The baudrate to operate the UART with.
     */
    UART(Pin txPin, Pin rxPin, uint32_t baudrate);

    /**
     * Set the baudrate that the UART will operate with.
     *
     * @param baudrate[in] The new baudrate to use
     */
    virtual void setBaudrate(uint32_t baudrate) = 0;

    /**
     * Set the format that data will be communicated with over UART.
     *
     * DEFAULT: 8 bit words, no parity, 1 stop bit
     *
     * @param wordLength[in] The number of bits in a work (5-8)
     * @param parity[in] The parity settings to use.
     * @param numStopBits[in] The number of stop bits (1-2)
     */
    virtual void setFormat(WordLength wordLength=WordLength::EIGHT,
            Parity parity=Parity::NONE,
            NumStopBits numStopBits=NumStopBits::ONE) = 0;

    /**
     * Sends a serial break condition over UART.
     */
    virtual void sendBreak() = 0;

    /**
     * Determins if the UART is currently readable.
     *
     * @return True if the UART is readable.
     */
    virtual bool isReadable() = 0;

    /**
     * Determins if the UART is currently writable.
     *
     * @return True if the UART is writable
     */
    virtual bool isWritable() = 0;

    /**
     * Put a single character to the UART module.
     *
     * @param c[in] The character to send over UART.
     */
    virtual void putc(char c) = 0;

    /**
     * Put a null-terminal string out over UART.
     *
     * @param s[in] The null-terminated string to put over UART.
     */
    virtual void puts(const char* s) = 0;

    /**
     * Blocking call to read a single character from the UART module.
     *
     * @return The character read in over UART.
     */
    virtual char getc() = 0;

    /**
     * Blocking call to get a string from the UART module. Stops when
     * a newline or EOF character is received or if the maximum size is
     * reached.
     *
     * @param buf The character array to fill
     * @param size The maximum number of characters to read.
     *
     * @return The buf pointer on success, NULL otherwise
     */
    char* gets(char* buf, size_t size);

    /**
     * Print a formatted string over UART, not great performance so best in
     * test situations not production.
     *
     * @param format The format string to print out.
     */
    virtual void printf(const char* format, ...) = 0;

    /**
     * Write out a single byte over UART.
     *
     * @param byte[in] The byte to write out over UART.
     */
    virtual void write(uint8_t byte) = 0;

    /**
     * Blocking reading of a single byte from the UART.
     *
     * @return The byte read in from UART.
     */
    virtual uint8_t read() = 0;

    /**
     * Write an arbitrary number of bytes out over UART.
     *
     * @param bytes[in] The data to send out over UART
     * @param size[in] The number of bytes to send out over UART.
     */
    virtual void writeBytes(uint8_t* bytes, size_t size) = 0;

    /**
     * Blocking reading of an arbitrary number of bytes in over UART.
     *
     * @param bytes[out] The data buffer to fill
     * @param size[in] The number of bytes to read in.
     */
    virtual void readBytes(uint8_t* bytes, size_t size) = 0;


protected:
    /// The TX pin used by the UART interface
    Pin txPin;
    /// The RX pin used by the UART interface
    Pin rxPin;
    /// The baudrate that is currently being operated at
    uint32_t baudrate;

    constexpr static uint32_t DEFAULT_TIMEOUT = 100;
};

}

#endif
