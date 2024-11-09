#include <core/rtos/Enums.hpp>
#include <core/rtos/Queue.hpp>

namespace core::rtos {

Queue::Queue(char* name, uint32_t messageSize, uint32_t numMessages)
    : name(name), messageSize(messageSize), queueSize(messageSize * numMessages) {}

TXError Queue::init(BytePoolBase& pool) {
    void* poolPointer;
    // allocate memory on the pool
    uint32_t errorCode = pool.allocateMemory(queueSize, NO_WAIT, &poolPointer);
    TXError error      = static_cast<TXError>(errorCode);
    if (error != SUCCESS)
        return error;
    // create the queue only if the memory allocation succeeds
    return static_cast<TXError>(tx_queue_create(&txQueue, name, messageSize, poolPointer, queueSize));
}

Queue::~Queue() {
    tx_queue_delete(&txQueue);
}

TXError Queue::flush() {
    return static_cast<TXError>(tx_queue_flush(&txQueue));
}

TXError Queue::prioritize() {
    return static_cast<TXError>(tx_queue_prioritize(&txQueue));
}

TXError Queue::receive(void* destination, uint32_t waitOption) {
    return static_cast<TXError>(tx_queue_receive(&txQueue, destination, waitOption));
}

TXError Queue::registerNotifyFunction(void (*notifyFunction)(Queue*)) {
    // TODO: registerNotifyFunction must be implemented
    return FEATURE_NOT_ENABLED;
}

TXError Queue::send(void* messagePointer, uint32_t waitOption) {
    return static_cast<TXError>(tx_queue_send(&txQueue, messagePointer, waitOption));
}

TXError Queue::sendToFront(void* messagePointer, uint32_t waitOption) {
    return static_cast<TXError>(tx_queue_front_send(&txQueue, messagePointer, waitOption));
}

TXError Queue::getName(char** name) {
    *name = this->name;
    return SUCCESS;
}

TXError Queue::getNumberOfEnqueuedMessages(uint32_t* numEnqueuedMessages) {
    uint32_t status = tx_queue_info_get(&txQueue, nullptr, numEnqueuedMessages, nullptr, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError Queue::getAvailableStorage(uint32_t* numAvailableMessages) {
    uint32_t status = tx_queue_info_get(&txQueue, nullptr, nullptr, numAvailableMessages, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError Queue::getNameOfFirstSuspendedThread(char** threadName) {
    TX_THREAD* thread;
    uint32_t status = tx_queue_info_get(&txQueue, nullptr, nullptr, nullptr, &thread, nullptr, nullptr);
    // exit early if the call failed
    if (status != SUCCESS) {
        return static_cast<TXError>(status);
    }

    // read the name off the struct
    status = tx_thread_info_get(thread, threadName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError Queue::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    uint32_t status = tx_queue_info_get(&txQueue, nullptr, nullptr, nullptr, nullptr, numSuspendedThreads, nullptr);
    return static_cast<TXError>(status);
}

} // namespace core::rtos
