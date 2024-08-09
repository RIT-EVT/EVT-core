
#include <EVT/rtos/Queue.hpp>

namespace core::rtos {

    Queue::Queue(const char* name, UINT messageSize, ULONG queueSize)
        : name(name), messageSize(messageSize), queueSize(queueSize) {}

    bool Queue::init(core::rtos::BytePool& pool) {
        void* poolPointer = pool.AllocateMemory(queueSize, TX_NO_WAIT);
        tx_queue_create(&txQueue, name, messageSize, poolPointer, queueSize);
    }

} //namespace core::rtos
