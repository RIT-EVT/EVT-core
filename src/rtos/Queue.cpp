
#include <EVT/rtos/Queue.hpp>

namespace core::rtos {

    Queue::Queue(const char* name, UINT messageSize, ULONG queueSize)
        : name(name), messageSize(messageSize), queueSize(queueSize) {}

    UINT Queue::destroy() {
        tx_queue_delete(&txQueue);
    }

    UINT Queue::flush() {
        tx_queue_flush(&txQueue);
    }

    UINT Queue::prioritize() {
        tx_queue_prioritize(&txQueue);
    }

    UINT Queue::recieve(void* destination, ULONG waitOption) {
        tx_queue_receive(&txQueue, destination, waitOption);
    }

    UINT Queue::registerSendNotifyFunction(core::rtos::Queue::notifyFunction_t notifyFunction) {
        tx_queue_send_notify(&txQueue, notifyFunction);
    }

    UINT Queue::send(void* source, ULONG waitOption) {
        tx_queue_send(&txQueue, source, waitOption);
    }

    UINT Queue::frontSend(void* source, ULONG waitOption) {
        tx_queue_front_send(&txQueue, source, waitOption);
    }

    bool Queue::init(core::rtos::BytePool& pool) {
        void* poolPointer = pool.AllocateMemory(queueSize, TX_NO_WAIT);
        tx_queue_create(&txQueue, name, messageSize, poolPointer, queueSize);
    }

} //namespace core::rtos
