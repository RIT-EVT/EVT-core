#ifndef EVT_RTOS_QUEUE_
#define EVT_RTOS_QUEUE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Queue : Initializable {
public:

    /**
     * Constructs a Queue object, but does not initiaize it (must call init before using).
     *
     * @param name The name of the queue.
     * @param messageSize Size (in 4-byte words) of each message in the queue.
     * @param queueSize Total size (in bytes) of the queue storage area.
     */
    Queue(const char* name, UINT messageSize,  ULONG queueSize);



    bool init(BytePool &pool) override;
private:

    TX_QUEUE txQueue;

    const char* name;

    UINT messageSize;

    ULONG queueSize;
};

} // namespace core::rtos

#endif//EVT_RTOS_QUEUE_
