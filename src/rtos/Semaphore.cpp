
#include <EVT/rtos/Semaphore.hpp>

namespace core::rtos {

Semaphore::Semaphore(char* name, uint32_t initialCount)
    : txSemaphore(), name(name), initialCount(initialCount) {}

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

TXError Semaphore::putNotify() {
    return static_cast<TXError>(tx_semaphore_put_notify(&txSemaphore, ));
}

TXError Semaphore::prioritize() {
    return static_cast<TXError>(tx_semaphore_prioritize(&txSemaphore));
}

TXError Semaphore::ceilingPut(uint32_t newCeiling) {
    return static_cast<TXError>(tx_semaphore_ceiling_put(&txSemaphore, newCeiling));
}

} //namespace core::rtos
