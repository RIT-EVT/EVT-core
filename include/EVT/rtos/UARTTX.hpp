#ifndef EVT_TX_UART_H
#define EVT_TX_UART_H

#include <EVT/rtos/Initializable.hpp>
#include <EVT/rtos/Threadx.hpp>
#include <EVT/rtos/Queue.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <HALf4/stm32f4xx.h>
#include <string>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

class UARTTX: public Initializable, IO::UART {
public:

    /**
     * Constructor for thread safe uart class.
     *
     * @param[in] uart A UART instance.
     */
    UARTTX(IO::UART&);

    TXError init(BytePoolBase &pool) override;

    void addQueuart(const char* message);

    void getQueuart();

    void printf(const char* format, ...);

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

    void write(uint8_t byte);

    uint8_t read();

    void writeBytes(uint8_t* bytes, size_t size);

    void readBytes(uint8_t* bytes, size_t size);

private:
    /// UART object
    IO::UART& copyUART;

    /// HAL representation of the UART
    UART_HandleTypeDef halUART;

    Queue queue;

    const char* txMessage;

    static UCHAR tx_byte_pool_buffer[65536];

    static TX_BYTE_POOL tx_app_byte_pool;
};

}// namespace core::rtos::wrapper

#endif//EVT_TX_UART_H
