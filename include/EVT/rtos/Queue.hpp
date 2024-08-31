#ifndef EVT_RTOS_QUEUE_
#define EVT_RTOS_QUEUE_

#include <EVT/rtos/Initializable.hpp>
#include <functional>

namespace core::rtos {

class Queue : Initializable {
public:

    /**
     * Constructs a Queue object, but does not initiaize it (must call init before using).\n\n
     *
     * NOTE: It is technically possible to overflow the size of the queue. If (messageSize * numMessages) is greater
     * than the max value of a uint32_t (roughly 4.2 billion), the calculation will overflow and your queue will be
     * incorrectly sized.
     *
     * @param[in] name The name of the queue.
     * @param[in] messageSize Size (in 4-byte words) of each message in the queue.
     * @param[in] numMessages Number of messages in the queue.
     */
    Queue(char* name, uint32_t messageSize,  uint32_t numMessages);

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

    TXError init(BytePoolBase& pool) override;
private:

    TX_QUEUE txQueue;

    char* name;

    uint32_t messageSize;

    uint32_t queueSize;

    void(*storedNotifyFunction)(Queue*);

    void memberNotifyFunction(TX_QUEUE* queue) {
        storedNotifyFunction(this);
    }

    /**
     * The type of notify function that threadx expects.
     */
    typedef void txNotifyFunction_t( TX_QUEUE * );

    /**
     * A pointer to the function that we will register with the threadx kernel when the
     * registerNotificationFunction method is called. This function calls memberNotifyFunction, which itself calls
     * storedNotifyFunction, which will be set to the passed-in function for the registerNotifyFunction method.
     */
    txNotifyFunction_t *txNotifyFunction;
};

} // namespace core::rtos

#endif//EVT_RTOS_QUEUE_
