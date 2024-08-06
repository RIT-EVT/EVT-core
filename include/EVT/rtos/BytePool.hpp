
#ifndef _EVT_RTOS_BYTEPOOL_
#define _EVT_RTOS_BYTEPOOL_

#include <cstdint>
#include <tx_api.h>

namespace core::rtos {

template <size_t SIZE>
class BytePool {
public:
    /**
     * Constructs a BytePool, including creating a buffer to hold the
     * information for the BytePool and the buffer for the pool itself.
     */
    BytePool();

    /**
     * Attempts to allocate a specific amount of memory from the this byte pool.
     * @param amount how much memory (in bytes) is requested to be allocated.
     * @param wait whether or not the calling thread should wait for enough memory to be available.
     * @return a pointer to the allocated memory.
     */
    void* AllocateMemory(size_t amount, bool wait);

private:
    UCHAR tx_byte_pool_buffer[SIZE];
    TX_BYTE_POOL tx_app_byte_pool;

};

} //namespace core::rtos


#endif //_EVT_RTOS_BYTEPOOL_
