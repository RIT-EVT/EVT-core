#include <EVT/rtos/UARTTX.hpp>
#include <cstdarg>
#include <cstdio>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

UARTTX::UARTTX(IO::UART& uart)
    : UART(uart), copyUART(uart), queue("UART queue", 4, 8) {
    /// queue( name of queue, size of each message, number of messages being sent )
}

//TODO: Get Rueuart to fix this.
TXError UARTTX::init(BytePoolBase &pool) {
    return queue.init(pool);
}

void UARTTX::addQueuart(const char* message){
    /**
     * 1. Check message.
     * 2. If message size is acceptable add it to queue.
     * 3. If message size is not acceptable return an error.
     */

    std::string s(message);

    if(s.size() != 4) {
        message = "queue Error";
        queue.send(&message, NoWait);
    }
    else {
        /// Add message to queue.
        queue.send(&message, NoWait);
        copyUART.printf("queue added");
    }
}

void UARTTX::getQueuart() {
    queue.receive(&txMessage, NoWait);
    copyUART.printf(txMessage);
}

void UARTTX::printf(const char* format, ...) {
    va_list args; /* Access the variable argument list */
    va_start(args, format); /* Tells the args variable to point to the format parameter first */
    char buffer[256]; /* Buffer array to hold the message */
    vsnprintf(buffer, sizeof(buffer), format, args); /* vsnprint formats the string and stores it in the buffer array */

    addQueuart(buffer);
    getQueuart();
//    copyUART.printf(buffer); /* Send the buffer data over UART */

    va_end(args); /* Cleans va_list once the message has been sent */
}

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