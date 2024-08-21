

#ifndef EVT_RTOS_SEMAPHORE_
#define EVT_RTOS_SEMAPHORE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Semaphore : Initializable {
public:
    /**
     * Constructs a Semaphore object, but does not initialize it (must call init before using).
     *
     * @param[in] name The name of the Semaphore.
     * @param[in] initialCount What number the Semaphore will start at.
     */
    Semaphore(char* name, uint32_t initialCount);

    /**
     * Semaphore Destructor.
     */
    ~Semaphore();

    TXError get(uint32_t waitOption);

    TXError put();

    TXError putNotify();

    TXError prioritize();

    TXError ceilingPut(uint32_t newCeiling);

    TXError init(BytePoolBase& pool) override;

private:
    /**
     * Threadx struct that actually holds all of the information for the Semaphore.
     */
    TX_SEMAPHORE txSemaphore;
    /**
     * The name of the Semaphore.
     */
    char* name;
    /**
     * The count the Semaphore will be initialized with.
     */
    const uint32_t initialCount;
};

} // namespace core::rtos

#endif//EVT_RTOS_SEMAPHORE_
