#include <core/rtos/UARTTX.hpp>
#include <core/utils/log.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstdint>

namespace log = EVT::core::log;
namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

/**
 * static definition for all the uarttx thread entry functions.
 * This should not need to be modified, but if you do modify it you must
 * NEVER EVER call uarttx->printf() or any method that calls that method in
 * this thread. See Notes in the UARTTX header comment for more information on why.
 *
 * @param[in] uarttx the specific uartttx object this entry function is running on.
 */
static void uartThreadEntryFunction(UARTTX* uarttx) {
    while (1) {
        uarttx->readQueuart();
    }
}

UARTTX::UARTTX(IO::UART& uart, std::size_t threadStackSize, uint32_t threadPriorityLevel,
               uint32_t threadPreemptThreshold, uint32_t threadTimeSlice)
    : UART(uart), copyUART(uart), queue((char*)"UARTTX Queue", UARTTX_QUEUE_MESSAGE_SIZE, UARTTX_QUEUE_NUM_MESSAGES),
      thread((char*)"UARTTX Thread", uartThreadEntryFunction, this, threadStackSize,
             threadPriorityLevel, threadPreemptThreshold, threadTimeSlice, true),
      readMutex((char*)"UARTTX Read Mutex", true) {
}

//TODO: Get Rueuart to fix this.
TXError UARTTX::init(BytePoolBase &pool) {
    TXError status = queue.init(pool);
    if (status != Success) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Errored on UARTTX Queue initialization. Error code %u", status);
        return static_cast<TXError>(status);
    }
    status = thread.init(pool);
    return static_cast<TXError>(status);
}

void UARTTX::printf(const char* format, ...) {
    va_list args; /* Access the variable argument list */
    va_start(args, format); /* Tells the args variable to point to the format parameter first */

    char buffer[256]; /* Buffer array to hold the message */
    vsnprintf(buffer, sizeof(buffer), format, args); /* vsnprint formats the string and stores it in the buffer array */

    //split longer messages into 64 (63 + null-termination) bit chunks.
    char temp[64];
    memset(temp, 0, 64); //clear the memory to 0 just in case
    uint32_t len = strlen(buffer);
    for (uint32_t i = 0; i < len; i+=63) {
        memccpy(temp, buffer+i, '\0', 63);
        temp[63] = '\0'; //set the last bit to the null terminator (should already be that but just in case)
        addQueuart(temp);
    }

    va_end(args); /* Cleans va_list once the message has been sent */
}

void UARTTX::addQueuart(char* buffer){
    queue.send(buffer, WaitForever);
}

void UARTTX::readQueuart() {
    char buffer[64]; /* Buffer array to hold the message - messages are at most 32 bytes long, but it gets angry if we set this buffer to be too small */
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
    readMutex.acquire(TXWait::WaitForever);
    uint8_t c;
    while (HAL_UART_Receive(&halUART, &c, 1, EVT_UART_TIMEOUT) == HAL_TIMEOUT) {
    }
    readMutex.release();
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
    readMutex.acquire(TXWait::WaitForever);
    HAL_UART_Receive(&halUART, bytes, size, EVT_UART_TIMEOUT);
    readMutex.release();
}

TXError UARTTX::getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages) {
    return queue.getNumberOfEnqueuedMessages(numEnqueuedMessages);
}

TXError UARTTX::getAvailableQueueStorage(uint32_t* numAvailableMessages) {
    return queue.getAvailableStorage(numAvailableMessages);
}

TXError UARTTX::getNameOfFirstSuspendedThread(char** threadName) {
    return queue.getNameOfFirstSuspendedThread(threadName);
}

TXError UARTTX::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    return queue.getNumSuspendedThreads(numSuspendedThreads);
}

}// namespace core::rtos::wrapper