
#include <EVT/rtos/Semaphore.hpp>

namespace core::rtos {

Semaphore::Semaphore(const char* name, uint32_t initialCount)
    : txSemaphore(), name(name), initialCount(initialCount) {}

Semaphore::~Semaphore() {
    tx_semaphore_delete(&txSemaphore);
}

TXError Semaphore::get(uint32_t waitOption) {
    return tx_semaphore_get(&txSemaphore);
}

TXError Semaphore::put() {
    return tx_semaphore_put(&txSemaphore);
}

TXError Semaphore::putNotify() {
    return tx_semaphore_put_notify(&txSemaphore);
}

TXError Semaphore::prioritize() {
    return tx_semaphore_prioritize(&txSemaphore);
}

TXError Semaphore::ceilingPut() {
    return tx_semaphore_ceiling_put(&txSemaphore);
}

TXError Semaphore::init(BytePoolBase& pool) {
    return tx_semaphore_create(&txSemaphore, name, initialCount);
}

} //namespace core::rtos
