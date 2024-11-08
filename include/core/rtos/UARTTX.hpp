#ifndef EVT_TX_UART_H
#define EVT_TX_UART_H

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

#ifndef UARTTX_QUEUE_MESSAGE_SIZE
    #define UARTTX_QUEUE_MESSAGE_SIZE 16
#endif // UARTTX_QUEUE_MESSAGE_SIZE

#ifndef UARTTX_QUEUE_NUM_MESSAGES
    #define UARTTX_QUEUE_NUM_MESSAGES 32
#endif // UARTTX_QUEUE_NUM_MESSAGES

#ifndef UARTTX_DEFAULT_STACK_SIZE
    #define UARTTX_DEFAULT_STACK_SIZE 1024
#endif // UARTTX_DEFAULT_STACK_SIZE

#ifndef UARTTX_DEFAULT_PRIORITY_LEVEL
    #define UARTTX_DEFAULT_PRIORITY_LEVEL 1u
#endif // UARTTX_DEFAULT_PRIORITY_LEVEL

#ifndef UARTTX_DEFAULT_PREEMPT_THRESHOLD
    #define UARTTX_DEFAULT_PREEMPT_THRESHOLD 0u
#endif // UARTTX_DEFAULT_PREEMPT_THRESHOLD

#ifndef UARTTX_DEFAULT_TIME_SLICE
    #define UARTTX_DEFAULT_TIME_SLICE MS_TO_TICKS(500)
#endif // UARTTX_DEFAULT_TIME_SLICE

namespace io = core::io;
namespace core::rtos::wrapper {

/**
 * Class that represents a threadsafe implementation of UART.
 * UARTTX uses a queue to buffer print statements sent to it. It then uses a thread to empty the queue periodically.\n\n
 *
 *
 * NOTE: The thread does not empty the queue when it is full. It simply waits until
 * the scheduler gives it priority to print the contents of the queue to UART.\n\n
 *
 *
 * NOTE: In some cases it may be possible that UARRTX never prints if there is
 * always a higher priority thread able to run, so it should be given as high a
 * priority as possible without it interfering with the normal execution of threads.\n\n
 *
 *
 * NOTE: UARRTX's own thread must NEVER print to UARTTX. If it does this while
 * the queue is full, it will suspend on the queue and since it is the only
 * thread that can empty the queue, it will never stop suspending. If this occurs,
 * any threads that then try to write to UARRTX will also suspend forever, which
 * will lead to your code slowly shutting down for no discernable reason.
 * There is no need to modify UARTTX's thread's method, so don't. Don't do it.
 */
class UARTTX : public Initializable, io::UART {
public:
    /**
     * Constructor for thread safe uart class.
     *
     * @param[in] uart A UART instance.
     * @param[in] threadStackSize the stack size of the UARTTX thread.
     * Setting it below the default size may cause issues.
     * @param[in] threadPriorityLevel The priority level of the UART Thread.
     * Setting it too low may result in UART never actually outputting.
     * @param[in] threadPreemptThreshold The preemption threshold of the UART thread.
     * Unless you absolutely need a thread to be able to interrupt this thread, do not
     * set this thread to a lower priority than the default because interrupting the thread
     * while it is running is likely to just cause the UART output to break immediately.
     * @param[in] threadTimeSlice The default minimum timeslice of this thread.
     */
    explicit UARTTX(io::UART& uart, std::size_t threadStackSize = UARTTX_DEFAULT_STACK_SIZE,
                    uint32_t threadPriorityLevel    = UARTTX_DEFAULT_PRIORITY_LEVEL,
                    uint32_t threadPreemptThreshold = UARTTX_DEFAULT_PREEMPT_THRESHOLD,
                    uint32_t threadTimeSlice        = UARTTX_DEFAULT_TIME_SLICE);

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

    // UARTTX Queue Informational Methods

    /**
     * Retrieves the number of enqueued messages in this UARTTX's Queue.
     *
     * @param[out] numEnqueuedMessages A pointer to store the number of enqueued messages in.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages);

    /**
     * Retrieves the number of more messages this UARTTX's Queue can fit.
     *
     * @param[out] numAvailableMessages A pointer to store the number of additional messages that the queue can fit.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getAvailableQueueStorage(uint32_t* numAvailableMessages);

    /**
     * Retrieves the name of the first thread suspended on sending a message to this UARTTX.
     *
     * @param[out] threadName a pointer to a place to store the name of the first suspended thread.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Retrieves the number of threads that are suspended on this UARTTX.
     *
     * @param[out] numSuspendedThreads a pointer to a place to store the number of suspended threads.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads);

    // must be public so the static method can call it
    /**
     * Reads from the queue and actually sends it to UART. Called by the UARTTX Thread's entry function.
     * Do not call this anywhere else.
     */
    void readQueuart();

private:
    void putc(char c) override;

    void puts(const char* s) override;

    void write(uint8_t byte) override;

    void writeBytes(uint8_t* bytes, size_t size) override;

    /**
     * Adds the given string to the Queue of messages to be sent to UART,
     * splitting it into multiple Queue messages if the string is too long
     * (each of the messages will be 16 words).
     *
     * @param[in] buffer a pointer to the string we are sending to the Queue.
     */
    void addQueuart(char* buffer);

    /** Pointer to store this thread's entry function */
    void (*threadEntryFunction)(UARTTX*);

    /** UART object */
    io::UART& copyUART;

    /** HAL representation of the UART */
    UART_HandleTypeDef halUART;

    /** Queue that buffers the messages to be sent to uart */
    Queue queue;

    /** Thread that empties the queue and prints it's contents to uart */
    Thread<UARTTX*> thread;

    /** Mutex that makes sure only one thread reads from UART at a time */
    Mutex readMutex;
};

} // namespace core::rtos::wrapper

#endif // EVT_TX_UART_H
