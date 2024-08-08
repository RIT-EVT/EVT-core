
#include <EVT/rtos/Semaphore.hpp>

namespace core::rtos {

Semaphore::Semaphore(const char* name, uint32_t initialCount)
    : txSemaphore(), name(name), initialCount(initialCount) {}

UINT Semaphore::destroy() {
    return tx_semaphore_delete(&txSemaphore);
}

UINT Semaphore::get(uint32_t waitOption) {
    return tx_semaphore_get(&txSemaphore);
}

UINT Semaphore::put() {
    return tx_semaphore_put(&txSemaphore);
}

UINT Semaphore::putNotify() {
    return tx_semaphore_put_notify(&txSemaphore);
}

UINT Semaphore::prioritize() {
    return tx_semaphore_prioritize(&txSemaphore);
}

UINT Semaphore::ceilingPut() {
    return tx_semaphore_ceiling_put(&txSemaphore);
}

bool Semaphore::init(core::rtos::BytePool& pool) {
    //TODO: instead of having the semaphore struct be within this class, should we place it in the BytePool?
    return tx_semaphore_create(&txSemaphore, name, initialCount) == TX_SUCCESS;
}

} //namespace core::rtos
