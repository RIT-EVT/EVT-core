#ifndef EVT_RTOS_MUTEX_
#define EVT_RTOS_MUTEX_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

/**
 * Class that wraps a Threadx Mutex. Mutexes are used to create mutual exclusion between threads.\n\n
 *
 * This class extends Initializable, and like all other Initializable classes must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 */
class Mutex : public Initializable {
public:
    /**
     * Constructs a Mutex object, but does not initialize it (must call init before using).
     *
     * @param[in] name A pointer to the name of the Mutex.
     * @param[in] priorityInheritance Whether or not threads currently holding this mutex should raise their priority
     * to that of the highest priority thread waiting for the mutex.
     */
    Mutex(char *name, bool priorityInheritance);

    TXError init(BytePoolBase &pool) override;

    /**
     * Mutex Destructor.
     */
    ~Mutex();

    /**
     * Attempts to obtain access to this Mutex. If the mutex is already in use by another thread,
     * the calling thread will wait for waitOption ticks for the mutex to be obtained.
     *
     * @param waitOption How long (in ticks) the calling thread should wait for the mutex to be available.
     * Use Enums::TXWait::WaitForever to wait forever.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError acquire(uint32_t waitOption);

    /**
     * Releases control of the mutex, allowing other threads to acquire it.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError release();

    /**
     * Prioritizes the list of waiting threads, placing the highest priority waiting thread at the front of the list.
     * (Does not effect the order of the other waiting threads).
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError prioritize();

private:
    /**
     * Threadx struct that actually holds all of the information for the Mutex.
     */
    TX_MUTEX txMutex;

    /**
     * Pointer to the name of the Mutex.
     */
    char* name;

    /**
     * Whether this mutex has priority inheritance.
     */
    const bool priorityInheritance;
};

} // namespace core::rtos

#endif //EVT_RTOS_MUTEX_
