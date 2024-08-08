
#include <EVT/rtos/Mutex.hpp>

namespace core::rtos {

Mutex::Mutex(const char* name, bool priorityInheritance)
    : txMutex(), name(name), priorityInheritance(priorityInheritance) {}

UINT Mutex::destroy() {
    return tx_mutex_delete(&txMutex);
}

UINT Mutex::get(uint32_t waitOption) {
    return tx_semaphore_get(&txMutex);
}

UINT Mutex::put() {
    return tx_semaphore_put(&txMutex);
}

UINT Mutex::prioritize() {
    return tx_semaphore_prioritize(&txMutex);
}

bool Mutex::init(BytePool &pool) {
    //TODO: create the txMutex struct on the BytePool instead of within the class?
    return tx_mutex_create(&txMutex, name, (UINT)priorityInheritance) == TX_SUCCESS;
}

} //namespace core::rtos
