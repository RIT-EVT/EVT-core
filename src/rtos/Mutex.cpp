
#include <EVT/rtos/Mutex.hpp>

namespace core::rtos {

Mutex::Mutex(char* name, bool priorityInheritance)
    : txMutex(), name(name), priorityInheritance(priorityInheritance) {}

TXError Mutex::init(BytePoolBase &pool) {
    return static_cast<TXError>(tx_mutex_create(&txMutex, name, (UINT)priorityInheritance));
}

Mutex::~Mutex() {
    tx_mutex_delete(&txMutex);
}

TXError Mutex::get(uint32_t waitOption) {
    return static_cast<TXError>(tx_mutex_get(&txMutex, waitOption));
}

TXError Mutex::put() {
    return static_cast<TXError>(tx_mutex_put(&txMutex));
}

TXError Mutex::prioritize() {
    return static_cast<TXError>(tx_mutex_prioritize(&txMutex));
}

} //namespace core::rtos
