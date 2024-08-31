
#include <EVT/rtos/BytePool.hpp>

namespace core::rtos {

template<std::size_t SIZE>
BytePool<SIZE>::BytePool(const char* name)
    : txBytePool(), name(name), buffer() {}

template<std::size_t SIZE>
BytePool<SIZE>::~BytePool() {
    tx_byte_pool_delete(&txBytePool);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::init() {
    return tx_byte_pool_create(&txBytePool, name, buffer, SIZE);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::allocateMemory(std::size_t amount, void** memoryPointer, uint32_t waitOption) {
    uint32_t errorCode = tx_byte_allocate(&txBytePool, memoryPointer, amount, waitOption);
    return static_cast<TXError>(errorCode);
}

} //namespace core::rtos
