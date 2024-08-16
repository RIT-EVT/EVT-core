
#include <EVT/rtos/Mutex.hpp>

namespace core::rtos {

Mutex::Mutex(const char* name, bool priorityInheritance)
    : txMutex(), name(name), priorityInheritance(priorityInheritance) {}

TXError Mutex::init(BytePoolBase &pool) {
    return tx_mutex_create(&txMutex, name, (UINT)priorityInheritance);
}

Mutex::~Mutex() {
    tx_mutex_delete(&txMutex);
}

TXError Mutex::get(uint32_t waitOption) {
    return tx_semaphore_get(&txMutex);
}

TXError Mutex::put() {
    return tx_semaphore_put(&txMutex);
}

TXError Mutex::prioritize() {
    return tx_semaphore_prioritize(&txMutex);
}

} //namespace core::rtos
