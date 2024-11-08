#include <core/rtos/Mutex.hpp>

namespace core::rtos {

Mutex::Mutex(char* name, bool priorityInheritance) : txMutex(), name(name), priorityInheritance(priorityInheritance) {}

TXError Mutex::init(BytePoolBase& pool) {
    return static_cast<TXError>(tx_mutex_create(&txMutex, name, (UINT) priorityInheritance));
}

Mutex::~Mutex() {
    tx_mutex_delete(&txMutex);
}

TXError Mutex::acquire(uint32_t waitOption) {
    return static_cast<TXError>(tx_mutex_get(&txMutex, waitOption));
}

TXError Mutex::release() {
    return static_cast<TXError>(tx_mutex_put(&txMutex));
}

TXError Mutex::prioritize() {
    return static_cast<TXError>(tx_mutex_prioritize(&txMutex));
}

TXError Mutex::getName(char** name) {
    *name = this->name;
    return SUCCESS;
}

TXError Mutex::getOwnershipCount(uint32_t* ownershipCount) {
    uint32_t status = tx_mutex_info_get(&txMutex, nullptr, ownershipCount, nullptr, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError Mutex::getNameOfOwner(char** ownerName) {
    TX_THREAD* owner;
    uint32_t status = tx_mutex_info_get(&txMutex, nullptr, nullptr, &owner, nullptr, nullptr, nullptr);
    // exit early if the call failed
    if (status != SUCCESS)
        return static_cast<TXError>(status);

    // read the name off the struct
    status = tx_thread_info_get(owner, ownerName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    return static_cast<TXError>(status);
}

TXError Mutex::getNameOfFirstSuspendedThread(char** threadName) {
    TX_THREAD* thread;
    uint32_t status = tx_mutex_info_get(&txMutex, nullptr, nullptr, nullptr, &thread, nullptr, nullptr);
    // exit early if the call failed
    if (status != SUCCESS)
        return static_cast<TXError>(status);

    // read the name off the struct
    status = tx_thread_info_get(thread, threadName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    return static_cast<TXError>(status);
}

TXError Mutex::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    uint32_t status = tx_mutex_info_get(&txMutex, nullptr, nullptr, nullptr, nullptr, numSuspendedThreads, nullptr);
    return static_cast<TXError>(status);
}

} // namespace core::rtos
