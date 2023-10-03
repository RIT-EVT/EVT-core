#ifndef _EVT_UARTf3xx_
#define _EVT_UARTf3xx_

#include <stdint.h>

#include <EVT/io/UART.hpp>

#include <HALf4/stm32f4xx.h>

namespace EVT::core::IO {

class UARTf4xx : public UART {
public:
    /**
     * Create an instance of the STMF302x8 UART interface using the provided
     * TX and RX pins.
     *
     * @param[in] txPin The UART TX pin.
     * @param[in] rxPin THe UART RX pin.
     * @param[in] baudrate The baudrate to operate the UART with/
     */
    UARTf4xx(Pin txPin, Pin rxPin, uint32_t baudrate);

    void setBaudrate(uint32_t baudrate);

    void setFormat(WordLength wordLength = WordLength::EIGHT,
                   Parity parity = Parity::NONE,
                   NumStopBits numStopBits = NumStopBits::ONE);

    void sendBreak();

    bool isReadable();

    bool isWritable();

    void putc(char c);

    void puts(const char* s);

    char getc();

    void printf(const char* format, ...);

    void write(uint8_t byte);

    uint8_t read();

    void writeBytes(uint8_t* bytes, size_t size);

    void readBytes(uint8_t* bytes, size_t size);

private:
    /// HAL representation of the UART
    UART_HandleTypeDef halUART;
};
}// namespace EVT::core::IO
#endif
