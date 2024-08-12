#ifndef EVT_RTOS_QUEUE_
#define EVT_RTOS_QUEUE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Queue : Initializable {
public:

    /**
     * A type that represents the function that can be registered to a queue
     * to notify when it is sent a message.
     */
    typedef void(*notifyFunction_t)(TX_QUEUE*);

    /**
     * Constructs a Queue object, but does not initiaize it (must call init before using).
     *
     * @param name The name of the queue.
     * @param messageSize Size (in 4-byte words) of each message in the queue.
     * @param queueSize Total size (in bytes) of the queue storage area.
     */
    Queue(const char* name, uint32_t messageSize,  uint32_t queueSize);

    /**
     * Queue Deconstructor.
     */
    ~Queue();

    TXError flush();

    TXError prioritize();

    TXError recieve(void* destination, uint32_t waitOption);

    /**
     * Register a function to be called whenever a message is successfully sent to the queue.
     *
     * @param notifyFunction The function to be called when a message is sent to the queue.
     * @return A Threadx status code representing the success of the method.
     */
    TXError registerSendNotifyFunction(notifyFunction_t notifyFunction);

    TXError send(void* source, uint32_t waitOption);

    TXError frontSend(void* source, uint32_t waitOption);

    TXError init(BytePoolBase& pool) override;
private:

    TX_QUEUE txQueue;

    const char* name;

    uint32_t messageSize;

    uint32_t queueSize;
};

} // namespace core::rtos

#endif//EVT_RTOS_QUEUE_
