
#include <EVT/rtos/Queue.hpp>

namespace core::rtos {

    Queue::Queue(const char* name, uint32_t messageSize, uint32_t queueSize)
        : name(name), messageSize(messageSize), queueSize(queueSize) {}

    TXError Queue::flush() {
        return tx_queue_flush(&txQueue);
    }

    TXError Queue::prioritize() {
        return tx_queue_prioritize(&txQueue);
    }

    TXError Queue::recieve(void* destination, uint32_t waitOption) {
        return tx_queue_receive(&txQueue, destination, waitOption);
    }

    TXError Queue::registerSendNotifyFunction(core::rtos::Queue::notifyFunction_t notifyFunction) {
        return tx_queue_send_notify(&txQueue, notifyFunction);
    }

    TXError Queue::send(void* source, uint32_t waitOption) {
        return tx_queue_send(&txQueue, source, waitOption);
    }

    TXError Queue::frontSend(void* source, uint32_t waitOption) {
        return tx_queue_front_send(&txQueue, source, waitOption);
    }

    TXError Queue::init(BytePoolBase& pool) {
        void* poolPointer = pool.AllocateMemory(queueSize, TX_NO_WAIT);
        return tx_queue_create(&txQueue, name, messageSize, poolPointer, queueSize);
    }

} //namespace core::rtos
