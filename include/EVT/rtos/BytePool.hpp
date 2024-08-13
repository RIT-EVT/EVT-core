/**
 * A template class that holds the actual information for managing a threadx bytepool.
 */

#ifndef _EVT_RTOS_BYTEPOOLBASE_
#define _EVT_RTOS_BYTEPOOLBASE_

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

    /**
     * BytePool deconstructor.
     */
    ~BytePool();

    TXError init() override;

    void* AllocateMemory(std::size_t amount, uint32_t waitOption) override;

private:
    /**
     * Buffer for the bytepool, SIZE bytes large.
     */
    uint8_t buffer[SIZE];

    /**
     * The struct that the threadx application uses to hold information about the bytepool.
     */
    TX_BYTE_POOL txBytePool;

    /**
     * A pointer to the name of the Bytepool.
     */
    const char* name;
};

} //namespace core::rtos


#endif //_EVT_RTOS_BYTEPOOLBASE_
