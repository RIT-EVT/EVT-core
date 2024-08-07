
#include <EVT/rtos/Semaphore.hpp>

namespace core::rtos {

Semaphore::Semaphore(ULONG initialCount, const char* name)
    : txSemaphoreStruct(), initialCount(initialCount), name(name) {}

bool Semaphore::init(core::rtos::BytePool& pool) {
    tx_semaphore_create(&txSemaphoreStruct, name, initialCount);
}

} //namespace core::rtos
