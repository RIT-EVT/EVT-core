
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
void* BytePool<SIZE>::allocateMemory(std::size_t amount, uint32_t waitOption) {
    void* output;
    tx_byte_allocate(&txBytePool, &output, amount, waitOption);
    //TODO: this implementation swallows the error code that tx_byte_allocate can return
    return output;
}

} //namespace core::rtos
