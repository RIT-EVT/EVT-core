#ifndef EVT_TX_UART_H
#define EVT_TX_UART_H

#include <EVT/rtos/Initializable.hpp>
#include <EVT/rtos/Threadx.hpp>
#include <EVT/rtos/Queue.hpp>
#include <EVT/rtos/Semaphore.hpp>
#include <EVT/rtos/Thread.hpp>
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
    explicit UARTTX(IO::UART& uart, std::size_t threadStackSize = 1024, uint32_t threadPriorityLevel = 1u,
           uint32_t threadPreemptThreshold = 0u, uint32_t threadTimeSlice = MS_TO_TICKS(500));

    TXError init(BytePoolBase &pool) override;

    void printf(const char* format, ...) override;

    void addQueuart(char* buffer, std::size_t size);

    void readQueuart();

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

    void write(uint8_t byte) override;

    uint8_t read() override;

    void writeBytes(uint8_t* bytes, size_t size) override;

    void readBytes(uint8_t* bytes, size_t size) override;

private:

    void (*threadEntryFunction)(UARTTX*);

    /// UART object
    IO::UART& copyUART;

    /// HAL representation of the UART
    UART_HandleTypeDef halUART;

    Queue queue;

    Thread<UARTTX*> thread;

    static UCHAR tx_byte_pool_buffer[65536];

    static TX_BYTE_POOL tx_app_byte_pool;
};

}// namespace core::rtos::wrapper

#endif//EVT_TX_UART_H
