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
                       uint32_t threadPreemptThreshold, uint32_t threadTimeSlice, uint32_t writeWaitOption)
    : Initializable((char*) "ThreadUART"), UART(uart), copyUART(uart),
      queue((char*) "ThreadUART Queue", THREADUART_QUEUE_MESSAGE_SIZE_WORDS, THREADUART_QUEUE_NUM_MESSAGES),
      thread((char*) "ThreadUART Thread", uartThreadEntryFunction, this, threadStackSize, threadPriorityLevel,
             threadPreemptThreshold, threadTimeSlice, true),
      readMutex((char*) "ThreadUART Read Mutex", true), writeMutex((char*) "ThreadUART Write Mutex", true),
        waitOption(writeWaitOption) {}

TXError ThreadUART::init(BytePoolBase& pool) {
    TXError status = queue.init(pool);
    if (status != TXE_SUCCESS) {
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
    buffer[255] = '\0';

    puts(buffer);
    va_end(args); /* Cleans va_list once the message has been sent */
}

void ThreadUART::puts(const char* s) {
    TXError status = writeMutex.get(waitOption);
    if (status != TXE_SUCCESS) {
        writeErrors++;
        return;
    }
    // split longer messages into 64 (63 + null-termination) bit chunks.
    char temp[THREADUART_QUEUE_MESSAGE_SIZE_BYTES];
    uint32_t len = strlen(s);
    for (uint32_t i = 0; i < len; i += THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1) {
        memccpy(temp, s + i, '\0', THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1);
        temp[THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1] = '\0'; // set the last bit to the null terminator
                                                              // (should already be that but just in case)
        status = queue.send(temp, waitOption);
        if (status != TXE_SUCCESS) {
            writeMutex.put();
            writeErrors++;
            return;
        }
    }
    writeMutex.put();
}

void ThreadUART::putc(char c) {
    TXError status = writeMutex.get(waitOption);
    if (status != TXE_SUCCESS) {
        writeErrors++;
        return;
    }
    char temp[THREADUART_QUEUE_MESSAGE_SIZE_BYTES];
    temp[0] = c;
    temp[1] = '\0';
    status = queue.send(temp, waitOption);
    if (status != TXE_SUCCESS) {
        writeErrors++;
    }
    writeMutex.put();
}

void ThreadUART::writeBytes(uint8_t* bytes, size_t size) {
    TXError status = writeMutex.get(waitOption);
    if (status != TXE_SUCCESS) {
        writeErrors++;
        return;
    }
    // split longer messages into THREADUART_QUEUE_MESSAGE_SIZE byte chunks.
    // we send THREADUART_QUEUE_MESSAGE_SIZE-1 bytes plus a null-terminating character, since UART is expecting a string
    char temp[THREADUART_QUEUE_MESSAGE_SIZE_BYTES];
    size_t i = 0;
    // send all the chunks except the last message (which might be less than THREADUART_QUEUE_MESSAGE_SIZE bytes)
    if (size > THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1) {
        size_t max = size - THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1;
        for (i = 0; i < max; i += THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1) {
            memcpy(temp, bytes + i, THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1);
            temp[THREADUART_QUEUE_MESSAGE_SIZE_BYTES - 1] = '\0';
            status = queue.send(temp, waitOption);
            if (status != TXE_SUCCESS) {
                writeMutex.put();
                writeErrors++;
                return;
            }
        }
    }
    // send the last amount of bytes
    size_t remaining_bytes = size - i;
    if (remaining_bytes > 0) {
        memset(temp, 0, THREADUART_QUEUE_MESSAGE_SIZE_BYTES);
        // copy last message into temp
        memcpy(temp, bytes + i, remaining_bytes);
        status = queue.send(temp, waitOption);
        if (status != TXE_SUCCESS) {
            writeErrors++;
        }
    }
    writeMutex.put();
}

void ThreadUART::write(uint8_t byte) {
    putc(static_cast<uint8_t>(byte));
}

void ThreadUART::sendFirstQueueMessage() {
    char buffer[THREADUART_QUEUE_MESSAGE_SIZE_BYTES]; // Buffer array to hold the message
    queue.receive(buffer, TXW_WAIT_FOREVER);          // Receives the message and assigns it to the buffer variable
    copyUART.writeBytes((uint8_t*) (buffer), strlen(buffer));
}

void ThreadUART::setBaudrate(uint32_t baudrate) {
    copyUART.setBaudrate(baudrate);
}

void ThreadUART::setFormat(WordLength wordLength, Parity parity, NumStopBits numStopBits) {
    copyUART.setFormat(wordLength, parity, numStopBits);
}

void ThreadUART::sendBreak() {
    copyUART.sendBreak();
}

bool ThreadUART::isReadable() {
    return copyUART.isReadable();
}

bool ThreadUART::isWritable() {
    return copyUART.isWritable();
}

char ThreadUART::getc() {
    readMutex.get(TXWait::TXW_WAIT_FOREVER);
    uint8_t c;
    c = copyUART.getc();
    readMutex.put();
    return static_cast<char>(c);
}

uint8_t ThreadUART::read() {
    return static_cast<uint8_t>(getc());
}

void ThreadUART::readBytes(uint8_t* bytes, size_t size) {
    readMutex.get(TXWait::TXW_WAIT_FOREVER);
    copyUART.readBytes(bytes, size);
    readMutex.put();
}

TXError ThreadUART::getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages) {
    return queue.getNumEnqueuedMessages(numEnqueuedMessages);
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

uint32_t ThreadUART::getNumWriteErrors() const {
    return writeErrors;
}

} // namespace core::rtos::tsio
