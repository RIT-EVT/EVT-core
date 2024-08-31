
#include <EVT/rtos/Queue.hpp>
#include <EVT/rtos/Enums.hpp>

namespace core::rtos {

Queue::Queue(char* name, uint32_t messageSize, uint32_t numMessages)
    : name(name), messageSize(messageSize), queueSize(messageSize*numMessages), storedNotifyFunction() {
    //We use bind to return a callable object that takes in only one argument, functionally removing the
    //implicit first argument that the memberNotifyFunction has.
    auto boundFunc = std::bind(&Queue::memberNotifyFunction, this, std::placeholders::_1);
    //We wrap this callable object into a wrapFunc so we can use .target on it.
    std::function<void(TX_QUEUE*)> wrapFunc = boundFunc;
    //We use the .target method to return a c-style function pointer that we can later pass to threadx
    //in the event that registerNotifyFunction is called.
    txNotifyFunction = wrapFunc.target<txNotifyFunction_t>();
}

TXError Queue::init(BytePoolBase& pool) {
    void* poolPointer;
    //allocate memory on the pool
    uint32_t errorCode = pool.allocateMemory(queueSize, &poolPointer, NoWait);
    TXError error = static_cast<TXError>(errorCode);
    if (error != Success) return error;
    //create the queue only if the memory allocation succeeds
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

TXError Queue::recieve(void* destination, uint32_t waitOption) {
    return static_cast<TXError>(tx_queue_receive(&txQueue, destination, waitOption));
}

TXError Queue::registerNotifyFunction(void(*notifyFunction)(Queue*)) {
    storedNotifyFunction = notifyFunction;
    return static_cast<TXError>(tx_queue_send_notify(&txQueue, txNotifyFunction));
}

TXError Queue::send(void* source, uint32_t waitOption) {
    return static_cast<TXError>(tx_queue_send(&txQueue, source, waitOption));
}

TXError Queue::frontSend(void* source, uint32_t waitOption) {
    return static_cast<TXError>(tx_queue_front_send(&txQueue, source, waitOption));
}

} //namespace core::rtos
