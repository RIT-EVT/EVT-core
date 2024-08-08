

#ifndef EVT_RTOS_SEMAPHORE_
#define EVT_RTOS_SEMAPHORE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Semaphore : Initializable {
public:
    /**
     * Constructs a Semaphore object, but does not initialize it (must call init before using).
     *
     * @param name The name of the Semaphore.
     * @param initialCount What number the Semaphore will start at.
     */
    Semaphore(const char* name, uint32_t initialCount);

    UINT destroy();

    UINT get(uint32_t waitOption);

    UINT put();

    UINT putNotify();

    UINT prioritize();

    UINT ceilingPut();

    bool init(BytePool &pool) override;

private:
    /**
     * Threadx struct that actually holds all of the information for the Semaphore.
     */
    TX_SEMAPHORE txSemaphore;
    /**
     * The name of the Semaphore.
     */
    const char* name;
    /**
     * The count the Semaphore will be initialized with.
     */
    const ULONG initialCount;
};

} // namespace core::rtos

#endif//EVT_RTOS_SEMAPHORE_
