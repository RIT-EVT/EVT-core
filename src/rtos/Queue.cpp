
#include <EVT/rtos/Queue.hpp>
#include <EVT/rtos/Enums.hpp>

namespace core::rtos {

Queue::Queue(const char* name, uint32_t messageSize, uint32_t queueSize)
    : name(name), messageSize(messageSize), queueSize(queueSize) {}

TXError Queue::init(BytePoolBase& pool) {
    void* poolPointer = pool.allocateMemory(queueSize, NoWait);
    return tx_queue_create(&txQueue, name, messageSize, poolPointer, queueSize);
}

Queue::~Queue() {
    tx_queue_delete(&txQueue);
}

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

} //namespace core::rtos
