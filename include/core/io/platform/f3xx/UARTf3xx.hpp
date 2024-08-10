#ifndef _EVT_UARTf3xx_
#define _EVT_UARTf3xx_

#include <cstdint>

#include <core/io/UART.hpp>

#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class UARTf3xx : public UART {
public:
    /**
     * Create an instance of the STMF3xx UART interface using the provided
     * TX and RX pins.
     *
     * @param[in] txPin The UART TX pin.
     * @param[in] rxPin THe UART RX pin.
     * @param[in] baudrate The baudrate to operate the UART with
     * @param[in] isSwapped Whether TX and RX should be swapped
     */
    UARTf3xx(Pin txPin, Pin rxPin, uint32_t baudrate, bool isSwapped);
    void setBaudrate(uint32_t baudrate) override;
    void setFormat(WordLength wordLength = WordLength::EIGHT,
                   Parity parity = Parity::NONE,
                   NumStopBits numStopBits = NumStopBits::ONE) override;

    void sendBreak() override;

    bool isReadable() override;
    bool isWritable() override;

    void putc(char c) override;
    void puts(const char* s) override;
    char getc() override;

    void printf(const char* format, ...) override;

    void write(uint8_t byte) override;
    uint8_t read() override;

    void writeBytes(uint8_t* bytes, size_t size) override;
    void readBytes(uint8_t* bytes, size_t size) override;

private:
    /// HAL representation of the UART
    UART_HandleTypeDef halUART;
};

}// namespace EVT::core::IO

#endif
