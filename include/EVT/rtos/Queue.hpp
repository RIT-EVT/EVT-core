#ifndef EVT_RTOS_QUEUE_
#define EVT_RTOS_QUEUE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Queue : Initializable {
public:

    /**
     * Constructs a Queue object, but does not initiaize it (must call init before using).
     *
     * @param[in] name The name of the queue.
     * @param[in] messageSize Size (in 4-byte words) of each message in the queue.
     * @param[in] queueSize Total size (in bytes) of the queue storage area.
     */
    Queue(const char* name, uint32_t messageSize,  uint32_t queueSize);

    TXError init(BytePoolBase& pool) override;

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
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError registerNotifyFunction(void(*notifyFunction)(Queue*));

    TXError send(void* source, uint32_t waitOption);

    TXError frontSend(void* source, uint32_t waitOption);
private:

    TX_QUEUE txQueue;

    const char* name;

    uint32_t messageSize;

    uint32_t queueSize;

    void(*storedNotifyFunction)(Queue*);

    void txNotifyFunction(TX_QUEUE* queue) {
        storedNotifyFunction(this);
    }
};

} // namespace core::rtos

#endif//EVT_RTOS_QUEUE_
