
#include <EVT/rtos/Semaphore.hpp>
#include <functional>

namespace core::rtos {

Semaphore::Semaphore(char* name, uint32_t initialCount)
    : txSemaphore(), name(name), initialCount(initialCount), storedNotifyFunction() {
    //We use bind to return a callable object that takes in only one argument, functionally removing the
    //implicit first argument that the memberNotifyFunction has.
    auto boundFunc = std::bind(&Semaphore::memberNotifyFunction, this, std::placeholders::_1);
    //We wrap this callable object into a wrapFunc so we can use .target on it.
    std::function<void(TX_SEMAPHORE *)> wrapFunc = boundFunc;
    //We use the .target method to return a c-style function pointer that we can later pass to threadx
    //in the event that registerNotifyFunction is called.
    txNotifyFunction = wrapFunc.target<txNotifyFunction_t>();
}

TXError Semaphore::init(BytePoolBase& pool) {
    return static_cast<TXError>(tx_semaphore_create(&txSemaphore, name, initialCount));
}

Semaphore::~Semaphore() {
    tx_semaphore_delete(&txSemaphore);
}

TXError Semaphore::get(uint32_t waitOption) {
    return static_cast<TXError>(tx_semaphore_get(&txSemaphore, waitOption));
}

TXError Semaphore::put() {
    return static_cast<TXError>(tx_semaphore_put(&txSemaphore));
}

TXError Semaphore::registerNotifyFunction(void(*notifyFunction)(Semaphore*)) {
    storedNotifyFunction = notifyFunction;
    return static_cast<TXError>(tx_semaphore_put_notify(&txSemaphore, txNotifyFunction));
}

TXError Semaphore::prioritize() {
    return static_cast<TXError>(tx_semaphore_prioritize(&txSemaphore));
}

TXError Semaphore::ceilingPut(uint32_t ceiling) {
    return static_cast<TXError>(tx_semaphore_ceiling_put(&txSemaphore, ceiling));
}

TXError Semaphore::getName(char** name) {
    *name = this->name;
    return Success;
}

TXError Semaphore::getCount(uint32_t* currentCount) {
    uint32_t status = tx_semaphore_info_get(&txSemaphore, nullptr, currentCount, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError Semaphore::getNameOfFirstSuspendedThread(char** threadName) {
    TX_THREAD *thread;
    uint32_t status = tx_semaphore_info_get(&txSemaphore, nullptr, nullptr, &thread, nullptr, nullptr);
    //exit early if the call failed
    if (status != Success)
        return static_cast<TXError>(status);

    //read the name off the struct
    status = tx_thread_info_get(thread, threadName, nullptr, nullptr, nullptr,
                                nullptr, nullptr, nullptr, nullptr);

    return static_cast<TXError>(status);
}

TXError Semaphore::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    uint32_t status = tx_semaphore_info_get(&txSemaphore, nullptr, nullptr, nullptr, numSuspendedThreads, nullptr);
    return static_cast<TXError>(status);
}


} //namespace core::rtos
