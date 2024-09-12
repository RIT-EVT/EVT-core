#ifndef _EVT_RTOS_BYTEPOOLBASE_
#define _EVT_RTOS_BYTEPOOLBASE_

#include <../../../libs/threadx/common/inc/tx_api.h>
#include <EVT/rtos/BytePoolBase.hpp>
#include <cstdint>

namespace core::rtos {

/**
 * Template class that wraps a Threadx BytePool. The BytePool can be used essentially as a heap,
 * via the allocateMemory() and releaseMemory() methods.
 * @tparam SIZE How large the bytepool is.
 */
template <std::size_t SIZE>
class BytePool : public BytePoolBase {
public:
    /**
     * Constructs a BytePoolBase, including creating a buffer to hold the
     * information for the pool and the buffer for the pool itself.
     *
     * @param[in] name A pointer to the name of the BytePool.
     */
    BytePool(char* name);

    /**
     * BytePool deconstructor.
     */
    ~BytePool();

    TXError releaseMemory(void* memoryPointer) override;

    TXError init() override;

    TXError allocateMemory(std::size_t amount, void** memoryPointer,uint32_t waitOption) override;

    TXError getAvailableBytes(uint32_t *availableBytes) override;

    TXError getFragments(uint32_t *fragments) override;

    TXError getNumSuspendedThreads(uint32_t *numSuspendedThreads) override;

    TXError getNameOfFirstSuspendedThread(char **name) override;

    TXError getName(char **name) override;

private:
    /**
     * A pointer to the name of the Bytepool.
     */
    char* name;

    /**
     * Buffer for the bytepool, SIZE bytes large.
     */
    uint8_t buffer[SIZE];

    /**
     * The struct that the threadx application uses to hold information about the bytepool.
     */
    TX_BYTE_POOL txBytePool;
};

} //namespace core::rtos


#endif //_EVT_RTOS_BYTEPOOLBASE_
