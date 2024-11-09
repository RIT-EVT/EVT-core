#include <core/rtos/Semaphore.hpp>

namespace core::rtos {

Semaphore::Semaphore(char* name, uint32_t initialCount) : name(name), txSemaphore(), initialCount(initialCount) {}

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

TXError Semaphore::registerNotifyFunction(void (*notifyFunction)(Semaphore*)) {
    // TODO: registerNotifyFunction must be implemented
    return FEATURE_NOT_ENABLED;
}

TXError Semaphore::prioritize() {
    return static_cast<TXError>(tx_semaphore_prioritize(&txSemaphore));
}

TXError Semaphore::putWithCeiling(uint32_t ceiling) {
    return static_cast<TXError>(tx_semaphore_ceiling_put(&txSemaphore, ceiling));
}

TXError Semaphore::getName(char** name) {
    *name = this->name;
    return SUCCESS;
}

TXError Semaphore::getCount(uint32_t* currentCount) {
    uint32_t status = tx_semaphore_info_get(&txSemaphore, nullptr, currentCount, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError Semaphore::getNameOfFirstSuspendedThread(char** threadName) {
    TX_THREAD* thread;
    uint32_t status = tx_semaphore_info_get(&txSemaphore, nullptr, nullptr, &thread, nullptr, nullptr);
    // exit early if the call failed
    if (status != SUCCESS) {
        return static_cast<TXError>(status);
    }

    // read the name off the struct
    status = tx_thread_info_get(thread, threadName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError Semaphore::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    uint32_t status = tx_semaphore_info_get(&txSemaphore, nullptr, nullptr, nullptr, numSuspendedThreads, nullptr);
    return static_cast<TXError>(status);
}

} // namespace core::rtos
