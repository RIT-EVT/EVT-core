
#include <EVT/rtos/Queue.hpp>
#include <../../../libs/threadx/common/inc/tx_api.h>

namespace core::rtos::wrapper {

Queue::Queue(TX_QUEUE& txQueue, CHAR* name, uint32_t messageSize, uint32_t queueSize)
    : txQueue(txQueue), name(name), messageSize(messageSize), queueSize(queueSize) {}

TXError Queue::init(BytePoolBase& pool) {
    void* poolPointer = pool.allocateMemory(queueSize, NoWait);
    myQueue = tx_queue_create(&txQueue, name, messageSize, poolPointer, queueSize);
    return static_cast<TXError>(myQueue);
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
    return static_cast<TXError>(tx_queue_receive(&txQueue, destination, waitOption));
}

TXError Queue::registerNotifyFunction(void(*notifyFunction)(Queue*)) {
    storedNotifyFunction = notifyFunction;
    return tx_queue_send_notify(&txQueue, txNotifyFunction);
}

TXError Queue::send(void* source, uint32_t waitOption) {
    return static_cast<TXError>(tx_queue_send(&txQueue, source, waitOption));
}

TXError Queue::frontSend(void* source, uint32_t waitOption) {
    return tx_queue_front_send(&txQueue, source, waitOption);
}

} //namespace core::rtos
