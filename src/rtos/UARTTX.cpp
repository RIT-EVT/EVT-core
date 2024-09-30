#include <EVT/rtos/UARTTX.hpp>
#include <cstdarg>
#include <cstdio>
#include <cstdint>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

/**
 * static definition for all the uarttx thread entry functions
 * @param[in] uarttx the specific uartttx object this
 */
static void uartThreadEntryFunction(UARTTX* uarttx) {
    /*
    this->readQueuart();
    this->printf("\n\rUARTTX: Thread_uart created\n\r");
    this->readQueuart();
    */

    rtos::sleep(S_TO_TICKS(1));

    while (1) {
        uarttx->readQueuart();
    }
}

UARTTX::UARTTX(IO::UART& uart, std::size_t threadStackSize, uint32_t threadPriorityLevel,
               uint32_t threadPreemptThreshold, uint32_t threadTimeSlice)
    : UART(uart), copyUART(uart), queue("UART Queue", 4, 8),
      thread("UART Thread", uartThreadEntryFunction, this, threadStackSize,
             threadPriorityLevel, threadPreemptThreshold, threadTimeSlice, true) {
}

//TODO: Get Rueuart to fix this.
TXError UARTTX::init(BytePoolBase &pool) {
    uint32_t status = queue.init(pool);
    if (status != TX_SUCCESS)
        return static_cast<TXError>(status);
    status = thread.init(pool);
    return static_cast<TXError>(status);
}

void UARTTX::printf(const char* format, ...) {
    va_list args; /* Access the variable argument list */
    va_start(args, format); /* Tells the args variable to point to the format parameter first */

    char buffer[256]; /* Buffer array to hold the message */
    vsnprintf(buffer, sizeof(buffer), format, args); /* vsnprint formats the string and stores it in the buffer array */

    addQueuart(buffer, sizeof(buffer)); /* Add formatted message to queue*/

    va_end(args); /* Cleans va_list once the message has been sent */
}

void UARTTX::addQueuart(char* buffer, std::size_t size){
    /**
     * 1. Check message.
     * 2. If message size is acceptable add it to queue.
     * 3. If message size is not acceptable return an error.
     */

    queue.send(buffer, WaitForever);
}

void UARTTX::readQueuart() {
    char buffer[256]; /* Buffer array to hold the message */
    queue.receive(buffer,WaitForever); /* Receives the message and assigns it to the buffer variable */
    copyUART.printf(buffer);
};

void UARTTX::setBaudrate(uint32_t baudrate) {
    this->halUART.Init.BaudRate = baudrate;
    this->baudrate = baudrate;
}

void UARTTX::setFormat(WordLength wordLength, Parity parity,
                         NumStopBits numStopBits) {
    halUART.Init.WordLength = static_cast<uint32_t>(wordLength);
    halUART.Init.Parity = static_cast<uint32_t>(parity);
    halUART.Init.Parity = static_cast<uint32_t>(numStopBits);
}

void UARTTX::sendBreak() {
    HAL_LIN_SendBreak(&halUART);
}

bool UARTTX::isReadable() {
    return halUART.pRxBuffPtr != NULL;
}

bool UARTTX::isWritable() {
    return halUART.pTxBuffPtr == NULL;
}

void UARTTX::putc(char c) {
    uint8_t* data = reinterpret_cast<uint8_t*>(&c);
    HAL_UART_Transmit(&halUART, data, 1, EVT_UART_TIMEOUT);
}

void UARTTX::puts(const char* s) {
    char buf[100];
    strncpy(buf, s, 100);
    uint8_t* data = reinterpret_cast<uint8_t*>(buf);
    HAL_UART_Transmit(&halUART, data, strlen(buf), EVT_UART_TIMEOUT);
}

char UARTTX::getc() {
    uint8_t c;
    while (HAL_UART_Receive(&halUART, &c, 1, EVT_UART_TIMEOUT) == HAL_TIMEOUT) {
    }
    return static_cast<char>(c);
}

void UARTTX::write(uint8_t byte) {
    putc(static_cast<uint8_t>(byte));
}

uint8_t UARTTX::read() {
    return static_cast<uint8_t>(getc());
}

void UARTTX::writeBytes(uint8_t* bytes, size_t size) {
    HAL_UART_Transmit(&halUART, bytes, size, EVT_UART_TIMEOUT);
}

void UARTTX::readBytes(uint8_t* bytes, size_t size) {
    HAL_UART_Receive(&halUART, bytes, size, EVT_UART_TIMEOUT);
}

}// namespace core::rtos::wrapper