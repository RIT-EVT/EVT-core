#ifndef _EVT_THREAD_UART_
#define _EVT_THREAD_UART_

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/rtos/Initializable.hpp>
#include <core/rtos/Mutex.hpp>
#include <core/rtos/Queue.hpp>
#include <core/rtos/Semaphore.hpp>
#include <core/rtos/Thread.hpp>
#include <core/rtos/Threadx.hpp>
#include <string>

// Defines the size for a ThreadUART Queue message in 4 byte words.
// This CANNOT be bigger than 16.
// Wrapped in an ifdef so this can be defined externally
#ifdef THREADUART_QUEUE_MESSAGE_SIZE_WORDS
    #if THREADUART_QUEUE_MESSAGE_SIZE_WORDS > 16
        #error THREADUART_QUEUE_MESSAGE_SIZE_WORDS must be less than 16.
    #endif
#else
    #define THREADUART_QUEUE_MESSAGE_SIZE_WORDS 16
#endif // THREADUART_QUEUE_MESSAGE_SIZE

//Defines the size for a ThreadUART Queue message in bytes.
#define THREADUART_QUEUE_MESSAGE_SIZE_BYTES (4*THREADUART_QUEUE_MESSAGE_SIZE_WORDS)

// Defines the number of message in the ThreadUART Queue.
// Wrapped in an ifndef so this can be defined externally
#ifndef THREADUART_QUEUE_NUM_MESSAGES
    #define THREADUART_QUEUE_NUM_MESSAGES 32
#endif // THREADUART_QUEUE_NUM_MESSAGES

#define THREADUART_DEFAULT_STACK_SIZE 1024

#define THREADUART_DEFAULT_PRIORITY_LEVEL 1u

#define THREADUART_DEFAULT_PREEMPT_THRESHOLD 0u

#define THREADUART_DEFAULT_TIME_SLICE MS_TO_TICKS(500)

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
     * Constructor for threadsafe UART class
     *
     * @param[in] uart A UART instance
     * @param[in] threadStackSize The stack size of the UARTTX thread.
     * Setting it below the default size may cause issues
     * @param[in] threadPriorityLevel The priority level of the UART Thread.
     * Setting it too low may result in UART never outputting until the Queue is completely full.
     * @param[in] threadPreemptThreshold The preemption threshold of the UART thread.
     * @param[in] threadTimeSlice The default minimum timeslice of this thread
     */
    explicit ThreadUART(io::UART& uart, std::size_t threadStackSize = THREADUART_DEFAULT_STACK_SIZE,
                        uint32_t threadPriorityLevel    = THREADUART_DEFAULT_PRIORITY_LEVEL,
                        uint32_t threadPreemptThreshold = THREADUART_DEFAULT_PREEMPT_THRESHOLD,
                        uint32_t threadTimeSlice        = THREADUART_DEFAULT_TIME_SLICE);

    // Inherited Initializable methods

    TXError init(BytePoolBase& pool) override;

    // Inherited UART methods

    /**
     * Add the given formatted string to the ThreadUART Queue to eventually be sent out over UART.
     * Only the first 256 characters of the result string of the formatting will be sent
     *
     * @param[in] format The format string to print out
     */
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
     * Copy the given byte array to the Queue of messages to be sent to UART, splitting it into multiple Queue
     * messages if the string is too long. Each of the messages will be 64 bytes
     *
     * @param[in] s The string we are sending to the Queue
     */
    void puts(const char* s);

    /**
     * Add a single character to the Queue of messages to be sent to UART. This single byte will take up an entire 64
     * byte message.
     * The charcater is encoded as a string consisting of the character and a null-terminating character
     *
     * @param c The character to be sent to the UART queue
     */
    void putc(char c) override;

    /**
     * Copy the given byte array to the Queue of messages to be sent to UART, splitting it into multiple Queue
     * messages if the string is too long. Each of the messages will be 64 bytes
     *
     * @param[in] bytes The byte array we are sending to the Queue
     * @param[in] size Size of the byte array
     */
    void writeBytes(uint8_t* bytes, size_t size) override;

    /**
     * Add a single byte to the Queue of messages to be sent to UART. This single byte will take up an entire 64 byte
     * message.
     * The byte is encoded as a string consisting of the byte and then immediately after a null-terminating character
     *
     * @param byte The byte to be sent to the UART Queue
     */
    void write(uint8_t byte) override;

    // UARTTX Queue Informational Methods

    /**
     * Get the number of enqueued messages in this ThreadUART's Queue
     *
     * @param[out] numEnqueuedMessages A pointer to store the number of enqueued messages in
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages);

    /**
     * Get the number of messages that this ThreadUART's Queue has remaining space for
     *
     * @param[out] numAvailableMessages The returned number of messages the Queue has remaining space for
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getAvailableQueueStorage(uint32_t* numAvailableMessages);

    /**
     * Get the name of the first thread suspended on sending a message to this ThreadUART
     *
     * @param[out] threadName The returned name of the first suspended thread
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Get the number of threads that are suspended on this ThreadUART
     *
     * @param[out] numSuspendedThreads The returned number of suspended threads
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads);

    // must be public so the static method can call it
    /**
     * Reads the first message from the queue and sends it to UART. Called by the ThreadUART Thread's entry function.
     * Do not call this anywhere else.
     */
    void sendFirstQueueMessage();

private:
    /** UART object */
    io::UART& copyUART;

    /** Queue that buffers the messages to be sent to uart */
    Queue queue;

    /** Thread that empties the queue and prints it's contents to uart */
    Thread<ThreadUART*> thread;

    /** Mutex that makes sure only one thread reads from UART at a time */
    Mutex readMutex;

    /** Mutex that makes sure only one thread writes to the Queue at a time */
    Mutex writeMutex;
};

} // namespace core::rtos::tsio

#endif // _EVT_THREAD_UART_
