#ifndef _EVT_THREAD_UART_
#define _EVT_THREAD_UART_

#include <HALf4/stm32f4xx.h>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/rtos/Initializable.hpp>
#include <core/rtos/Mutex.hpp>
#include <core/rtos/Queue.hpp>
#include <core/rtos/Semaphore.hpp>
#include <core/rtos/Thread.hpp>
#include <core/rtos/Threadx.hpp>
#include <string>

// All defines wrapped in ifndefs so we can set them externally.

#ifndef THREADUART_QUEUE_MESSAGE_SIZE
    #define THREADUART_QUEUE_MESSAGE_SIZE 16
#endif // THREADUART_QUEUE_MESSAGE_SIZE

#ifndef THREADUART_QUEUE_NUM_MESSAGES
    #define THREADUART_QUEUE_NUM_MESSAGES 32
#endif // THREADUART_QUEUE_NUM_MESSAGES

#ifndef THREADUART_DEFAULT_STACK_SIZE
    #define THREADUART_DEFAULT_STACK_SIZE 1024
#endif // THREADUART_DEFAULT_STACK_SIZE

#ifndef THREADUART_DEFAULT_PRIORITY_LEVEL
    #define THREADUART_DEFAULT_PRIORITY_LEVEL 1u
#endif // THREADUART_DEFAULT_PRIORITY_LEVEL

#ifndef THREADUART_DEFAULT_PREEMPT_THRESHOLD
    #define THREADUART_DEFAULT_PREEMPT_THRESHOLD 0u
#endif // THREADUART_DEFAULT_PREEMPT_THRESHOLD

#ifndef THREADUART_DEFAULT_TIME_SLICE
    #define THREADUART_DEFAULT_TIME_SLICE MS_TO_TICKS(500)
#endif // THREADUART_DEFAULT_TIME_SLICE

namespace io = core::io;
namespace core::rtos::tsio {

/**
 * Class that represents a threadsafe implementation of UART.
 * UARTTX uses a queue to buffer print statements sent to it. It then uses a thread to empty the queue periodically.\n\n
 *
 *
 * NOTE: The thread does not empty the queue when it is full. It simply waits until
 * the scheduler gives it priority to print the contents of the queue to UART.\n\n
 *
 *
 * NOTE: In some cases it may be possible that ThreadUART never prints if there is
 * always a higher priority thread able to run, so it should be given as high a
 * priority as possible without it interfering with the normal execution of threads.\n\n
 *
 *
 * NOTE: ThreadUART's own thread must NEVER print to ThreadUART. If it does this while
 * the queue is full, it will suspend on the queue and since it is the only
 * thread that can empty the queue, it will never stop suspending. If this occurs,
 * any threads that then try to write to ThreadUART will also suspend forever, which
 * will lead to your code slowly shutting down for no clear reason.
 * There should be no need to modify ThreadUART's thread's method.
 */
class ThreadUART : public Initializable, public io::UART {
public:
    /**
     * Constructor for thread safe uart class
     *
     * @param[in] uart A UART instance
     * @param[in] threadStackSize The stack size of the UARTTX thread.
     * Setting it below the default size may cause issues
     * @param[in] threadPriorityLevel The priority level of the UART Thread.
     * Setting it too low may result in UART never actually outputting
     * @param[in] threadPreemptThreshold The preemption threshold of the UART thread.
     * Unless you absolutely need a thread to be able to interrupt this thread, do not
     * set this thread to a lower priority than the default because interrupting the thread
     * while it is running is likely to just cause the UART output to break immediately.
     * @param[in] threadTimeSlice The default minimum timeslice of this thread
     */
    explicit ThreadUART(io::UART& uart, std::size_t threadStackSize = THREADUART_DEFAULT_STACK_SIZE,
                    uint32_t threadPriorityLevel    = THREADUART_DEFAULT_PRIORITY_LEVEL,
                    uint32_t threadPreemptThreshold = THREADUART_DEFAULT_PREEMPT_THRESHOLD,
                    uint32_t threadTimeSlice        = THREADUART_DEFAULT_TIME_SLICE);

    // Inherited Initializable methods

    TXError init(BytePoolBase& pool) override;

    // Inherited UART methods

    void printf(const char* format, ...) override;

    void setBaudrate(uint32_t baudrate) override;

    void setFormat(WordLength wordLength = WordLength::EIGHT, Parity parity = Parity::NONE,
                   NumStopBits numStopBits = NumStopBits::ONE) override;

    void sendBreak() override;

    bool isReadable() override;

    bool isWritable() override;

    char getc() override;

    uint8_t read() override;

    void readBytes(uint8_t* bytes, size_t size) override;

    /**
     * Copies the given string to the Queue of messages to be sent to UART,
     * splitting it into multiple Queue messages if the string is too long
     * (each of the messages will be 64 bytes)
     *
     * @param[in] s A pointer to the string we are sending to the Queue
     */
    void puts(const char* s);

    void putc(char c) override;

    /**
     * Copies the given byte array to the Queue of messages to be sent to UART,
     * splitting it into multiple Queue messages if the string is too long
     * (each of the messages will be 64 bytes).
     *
     * @param[in] bytes pointer to the byte array we are sending to the Queue
     * @param[in] size size of the byte array
     */
    void writeBytes(uint8_t* bytes, size_t size) override;

    void write(uint8_t byte) override;

    // UARTTX Queue Informational Methods

    /**
     * Retrieve the number of enqueued messages in this ThreadUART's Queue
     *
     * @param[out] numEnqueuedMessages A pointer to store the number of enqueued messages in
     * @return The first error found by the function or Success if there was no error
     */
    TXError getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages);

    /**
     * Retrieve the number of more messages this ThreadUART's Queue can fit
     *
     * @param[out] numAvailableMessages A pointer to store the number of additional messages that the queue can fit
     * @return The first error found by the function or Success if there was no error
     */
    TXError getAvailableQueueStorage(uint32_t* numAvailableMessages);

    /**
     * Retrieve the name of the first thread suspended on sending a message to this ThreadUART
     *
     * @param[out] threadName A pointer to a place to store the name of the first suspended thread
     * @return The first error found by the function or Success if there was no error
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Retrieve the number of threads that are suspended on this ThreadUART
     *
     * @param[out] numSuspendedThreads A pointer to a place to store the number of suspended threads
     * @return The first error found by the function or Success if there was no error
     */
    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads);

    // must be public so the static method can call it
    /**
     * Reads the first message from the queue and sends it to UART. Called by the ThreadUART Thread's entry function.
     * Do not call this anywhere else.
     */
    void sendFirstQueueMessage();

private:

    /** Pointer to store this thread's entry function */
    void (*threadEntryFunction)(ThreadUART*);

    /** UART object */
    io::UART& copyUART;

    /** HAL representation of the UART */
    UART_HandleTypeDef halUART;

    /** Queue that buffers the messages to be sent to uart */
    Queue queue;

    /** Thread that empties the queue and prints it's contents to uart */
    Thread<ThreadUART*> thread;

    /** Mutex that makes sure only one thread reads from UART at a time */
    Mutex readMutex;
};

} // namespace core::rtos::tsio

#endif // _EVT_THREAD_UART_
