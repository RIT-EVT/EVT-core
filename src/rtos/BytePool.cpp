
#include <EVT/rtos/BytePool.hpp>

namespace core::rtos {

    template<size_t SIZE>
    BytePool<SIZE>::BytePool() {
        if(tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, SIZE) != TX_SUCCESS) {
            //TODO: THROW AN ERROR SOMEHOW
        }
    }

    template <size_t SIZE>
    void* BytePool<SIZE>::AllocateMemory(size_t amount, bool wait) {
        void* output;
        tx_byte_allocate(&tx_app_byte_pool, &output, amount, wait ? TX_WAIT_FOREVER : TX_NO_WAIT);
        return output;
    }
} //namespace core::rtos
