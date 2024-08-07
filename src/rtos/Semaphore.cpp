
#include <EVT/rtos/Semaphore.hpp>

namespace core::rtos {

Semaphore::Semaphore(ULONG initialCount, const char* name)
    : txSemaphore(), initialCount(initialCount), name(name) {}

UINT Semaphore::destroy() {
    return tx_semaphore_delete(&txSemaphore);
}

UINT Semaphore::get() {
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
    return tx_semaphore_create(&txSemaphore, name, initialCount) == TX_SUCCESS;
}

} //namespace core::rtos
