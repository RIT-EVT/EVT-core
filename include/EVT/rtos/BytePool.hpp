/**
 * A template class that holds the actual information for managing a threadx bytepool.
 */

#ifndef _EVT_RTOS_BYTEPOOLBASE_
#define _EVT_RTOS_BYTEPOOLBASE_

#include <../../../libs/threadx/common/inc/tx_api.h>
#include <../../../libs/threadx/ports/cortex_m4/gnu/inc/tx_port.h>
#include <EVT/rtos/BytePoolBase.hpp>
#include <cstdint>

namespace core::rtos {

template <std::size_t SIZE>
class BytePool : BytePoolBase {
public:
    /**
     * Constructs a BytePoolBase, including creating a buffer to hold the
     * information for the pool and the buffer for the pool itself.
     *
     * @param[in] name A pointer to the name of the BytePool.
     */
    BytePool(const char* name);

    TXError init() override;

    void* AllocateMemory(std::size_t amount, uint32_t waitOption) override;

private:
    /**
     * Buffer for the bytepool, SIZE bytes large.
     */
    uint8_t tx_byte_pool_buffer[SIZE];

    /**
     * The struct that the threadx application uses to hold information about the bytepool.
     */
    TX_BYTE_POOL tx_app_byte_pool;

    /**
     * A pointer to the name of the Bytepool.
     */
    const char* name;
};

} //namespace core::rtos


#endif //_EVT_RTOS_BYTEPOOLBASE_
