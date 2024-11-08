#ifndef EVT_RTOS_QUEUE_
#define EVT_RTOS_QUEUE_

#include <core/rtos/Initializable.hpp>
#include <functional>

namespace core::rtos {

/**
 * Class that wraps a ThreadX Queue. Queues are a FIFO data structure, and ThreadX queues provide additional
 * functionality for interacting with queues in a threaded environment.
 * \n\n
 *
 * This class extends Initializable, and like all other Initializable classes must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 */
class Queue : public Initializable {
public:
    /**
     * Construct a Queue object, but do not initialize it (must call init before using)\n\n
     *
     * NOTE: It is technically possible to overflow the size of the queue. If (messageSize * numMessages) is greater
     * than the max value of a uint32_t (roughly 4.2 billion), the calculation will overflow and your queue will be
     * incorrectly sized. Take care to ensure this does not happen.
     *
     * @param[in] name The name of the queue
     * @param[in] messageSize Size (in 4-byte words) of each message in the queue. The message size can be at most 16
     * @param[in] numMessages Number of messages in the queue
     */
    Queue(char* name, uint32_t messageSize, uint32_t numMessages);

    /**
     * Queue Deconstructor
     */
    ~Queue();

    TXError init(BytePoolBase& pool) override;

    /**
     * Flush the queue, emptying all of the messages out of it
     *
     * @return The first error found by the function or Success if there was no error
     */
    TXError flush();

    /**
     * Prioritize the list of waiting threads, placing the highest priority waiting thread at the front of the list.
     * (Does not effect the order of the other waiting threads)
     *
     * @return The first error found by the function or Success if there was no error
     */
    TXError prioritize();

    /**
     * Attempt to pop a message from the queue. If the queue is empty, the calling thread will suspend for
     * waitOption ticks for the queue to be sent a message
     *
     * @param[out] destination pointer to the location for the popped message to be stored
     * @param[in] waitOption How long (in ticks) the calling thread should wait for the mutex to be available.
     * Use Enums::TXWait::WaitForever to wait forever
     * @return The first error found by the function or Success if there was no error
     */
    TXError receive(void* destination, uint32_t waitOption);

    /**
     * Register a function to be called whenever a message is successfully sent to the queue
     *
     * @param notifyFunction The function to be called when a message is sent to the queue
     * @return The first error found by the function or Success if there was no error
     */
    TXError registerNotifyFunction(void (*notifyFunction)(Queue*));

    /**
     * Attempt to send a message to the queue. If the queue is full, the calling thread will suspend for
     * waitOption ticks for the queue to empty enough to send the message
     *
     * @param[in] messagePointer A pointer to the message that will be copied to the queue
     * @param[in] waitOption How long (in ticks) the calling thread should wait for the mutex to be available.
     * Use Enums::TXWait::WaitForever to wait forever
     * @return The first error found by the function or Success if there was no error
     */
    TXError send(void* messagePointer, uint32_t waitOption);

    /**
     * Attempt to send a message to the front of the queue. If the queue is full,
     * the calling thread will suspend for waitOption ticks for the queue to empty enough to send the message
     *
     * @param[in] messagePointer A pointer to the message that will be copied to the queue
     * @param[in] waitOption How long (in ticks) the calling thread should wait for the mutex to be available.
     * Use Enums::TXWait::WaitForever to wait forever
     * @return The first error found by the function or Success if there was no error
     */
    TXError sendToFront(void* messagePointer, uint32_t waitOption);

    // Getters

    /**
     * Retrieve the name of this Queue
     *
     * @param[out] name A pointer to a place to store the name pointer
     * @return The first error found by the function or Success if there was no error
     */
    TXError getName(char** name);

    /**
     * Retrieve the number of enqueued messages in this Queue
     *
     * @param[out] numEnqueuedMessages A pointer to a place to store the number of enqueued messages
     * @return The first error found by the function or Success if there was no error
     */
    TXError getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages);

    /**
     * Retrieve the number of more messages the Queue can fit
     *
     * @param[out] numAvailableMessages A pointer to the place to store the number of more messages the queue can fit
     * @return The first error found by the function or Success if there was no error
     */
    TXError getAvailableStorage(uint32_t* numAvailableMessages);

    /**
     * Retrieve the name of the first suspended thread
     *
     * @param[out] threadName A pointer to a place to store the name of the first suspended thread
     * @return The first error found by the function or Success if there was no error
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Retrieve the number of threads that are suspended on this Queue
     *
     * @param[out] numSuspendedThreads a pointer to a place to store the number of suspended threads
     * @return The first error found by the function or Success if there was no error
     */
    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads);

private:
    /** The name of this object */
    char* name;

    /** The threadx struct that represents this object in the threadx kernel */
    TX_QUEUE txQueue;

    /** How large (in words) each message in the queue is */
    uint32_t messageSize;

    /** How large the entire queue is (calculated at constructor by messageSize * numMessages) */
    uint32_t queueSize;
};

} // namespace core::rtos

#endif // EVT_RTOS_QUEUE_
