#include <core/rtos/tsio/ThreadUART.hpp>
#include <core/utils/log.hpp>

#include <cstdarg>
#include <cstdint>
#include <cstdio>

namespace log = core::log;
namespace io  = core::io;
namespace core::rtos::tsio {

// empty namespace to isolate this entry function to this file only
namespace {

/**
 * static definition for all the threaduart thread entry functions.
 * This should not need to be modified, but if you do modify it you must
 * NEVER EVER call threadUart->printf() or any method that calls that method in
 * this thread. See Notes in the ThreadUART header comment for more information on why.
 *
 * @param[in] threadUART the specific ThreadUART object this entry function is running on.
 */
[[noreturn]] static void uartThreadEntryFunction(ThreadUART* threadUART) {
    while (true) {
        threadUART->sendFirstQueueMessage();
    }
}

} // namespace

ThreadUART::ThreadUART(io::UART& uart, std::size_t threadStackSize, uint32_t threadPriorityLevel,
                       uint32_t threadPreemptThreshold, uint32_t threadTimeSlice)
    : UART(uart), copyUART(uart),
      queue((char*) "ThreadUART Queue", THREADUART_QUEUE_MESSAGE_SIZE, THREADUART_QUEUE_NUM_MESSAGES),
      thread((char*) "ThreadUART Thread", uartThreadEntryFunction, this, threadStackSize, threadPriorityLevel,
             threadPreemptThreshold, threadTimeSlice, true),
      readMutex((char*) "ThreadUART Read Mutex", true),
      writeMutex((char*) "ThreadUART Write Mutex", true) {}

TXError ThreadUART::init(BytePoolBase& pool) {
    TXError status = queue.init(pool);
    if (status != SUCCESS) {
        log::LOGGER.log(
            log::Logger::LogLevel::ERROR, "Errored on ThreadUART Queue initialization. Error code %u\n", status);
        return static_cast<TXError>(status);
    }
    status = thread.init(pool);
    return static_cast<TXError>(status);
}

void ThreadUART::printf(const char* format, ...) {
    va_list args;           /* Access the variable argument list */
    va_start(args, format); /* Tells the args variable to point to the format parameter first */

    char buffer[256];                                /* Buffer array to hold the message */
    vsnprintf(buffer, sizeof(buffer), format, args); /* vsnprint formats the string and stores it in the buffer array */

    puts(buffer);

    va_end(args); /* Cleans va_list once the message has been sent */
}

void ThreadUART::puts(const char* s) {
    writeMutex.get(TX_WAIT_FOREVER);
    // split longer messages into 64 (63 + null-termination) bit chunks.
    char temp[64];
    uint32_t len = strlen(s);
    for (uint32_t i = 0; i < len; i += 63) {
        memccpy(temp, s + i, '\0', 63);
        temp[63] = '\0'; // set the last bit to the null terminator (should already be that but just in case)
        queue.send(temp, WAIT_FOREVER);
    }
    writeMutex.put();
}

void ThreadUART::putc(char c) {
    writeMutex.get(TX_WAIT_FOREVER);
    char temp[64];
    temp[0] = c;
    temp[1] = '\0';
    queue.send(temp, WAIT_FOREVER);
    writeMutex.put();
}

void ThreadUART::writeBytes(uint8_t* bytes, size_t size) {
    writeMutex.get(TX_WAIT_FOREVER);
    // split longer messages into 64 bit chunks.
    // we send 63 bytes plus a null-terminating character, since UART is expecting a string
    char temp[64];
    size_t i = 0;
    // send all the chunks except the last message (which might be less than 64 bytes)
    if (size > 63) {
        size_t max = size - 63;
        for (i = 0; i < max; i += 63) {
            memcpy(temp, bytes + i, 63);
            temp[63] = '\0';
            queue.send(temp, WAIT_FOREVER);
        }
    }
    // send the last amount of bytes
    size_t remaining_bytes = size - i;
    if (remaining_bytes > 0) {
        memset(temp, 0, 64);
        // copy last message into temp
        memcpy(temp, bytes + i, remaining_bytes);
        queue.send(temp, WAIT_FOREVER);
    }
    writeMutex.put();
}

void ThreadUART::write(uint8_t byte) {
    putc(static_cast<uint8_t>(byte));
}

void ThreadUART::sendFirstQueueMessage() {
    char buffer[64];                     // Buffer array to hold the message
    queue.receive(buffer, WAIT_FOREVER); // Receives the message and assigns it to the buffer variable
    copyUART.writeBytes((uint8_t*) (buffer), strlen(buffer));
}

void ThreadUART::setBaudrate(uint32_t baudrate) {
    this->halUART.Init.BaudRate = baudrate;
    this->baudrate              = baudrate;
}

void ThreadUART::setFormat(WordLength wordLength, Parity parity, NumStopBits numStopBits) {
    halUART.Init.WordLength = static_cast<uint32_t>(wordLength);
    halUART.Init.Parity     = static_cast<uint32_t>(parity);
    halUART.Init.Parity     = static_cast<uint32_t>(numStopBits);
}

void ThreadUART::sendBreak() {
    HAL_LIN_SendBreak(&halUART);
}

bool ThreadUART::isReadable() {
    return halUART.pRxBuffPtr != NULL;
}

bool ThreadUART::isWritable() {
    return halUART.pTxBuffPtr == NULL;
}

char ThreadUART::getc() {
    readMutex.get(TXWait::WAIT_FOREVER);
    uint8_t c;
    while (HAL_UART_Receive(&halUART, &c, 1, EVT_UART_TIMEOUT) == HAL_TIMEOUT) {}
    readMutex.put();
    return static_cast<char>(c);
}

uint8_t ThreadUART::read() {
    return static_cast<uint8_t>(getc());
}

void ThreadUART::readBytes(uint8_t* bytes, size_t size) {
    readMutex.get(TXWait::WAIT_FOREVER);
    HAL_UART_Receive(&halUART, bytes, size, EVT_UART_TIMEOUT);
    readMutex.put();
}

TXError ThreadUART::getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages) {
    return queue.getNumberOfEnqueuedMessages(numEnqueuedMessages);
}

TXError ThreadUART::getAvailableQueueStorage(uint32_t* numAvailableMessages) {
    return queue.getAvailableStorage(numAvailableMessages);
}

TXError ThreadUART::getNameOfFirstSuspendedThread(char** threadName) {
    return queue.getNameOfFirstSuspendedThread(threadName);
}

TXError ThreadUART::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    return queue.getNumSuspendedThreads(numSuspendedThreads);
}

} // namespace core::rtos::tsio
