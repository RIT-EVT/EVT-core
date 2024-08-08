#ifndef EVT_RTOS_MUTEX_
#define EVT_RTOS_MUTEX_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Mutex : Initializable {
public:
    /**
     * Constructs a Mutex object, but does not initialize it (must call init before using).
     *
     * @param[in] name A pointer to the name of the Mutex.
     * @param[in] priorityInheritance Whether or not threads currently holding this mutex should raise their priority
     * to that of the highest priority thread waiting for the mutex.
     */
    Mutex(const char *name, bool priorityInheritance);

    UINT destroy();

    UINT get(uint32_t waitOption);

    UINT put();

    UINT prioritize();

    bool init(BytePool &pool) override;

private:
    /**
     * Threadx struct that actually holds all of the information for the Mutex.
     */
    TX_MUTEX txMutex;

    /**
     * Pointer to the name of the Mutex.
     */
    const char* name;

    /**
     * Whether this mutex has priority inheritance.
     */
    const bool priorityInheritance;
};

} // namespace core::rtos

#endif//EVT_RTOS_MUTEX_
