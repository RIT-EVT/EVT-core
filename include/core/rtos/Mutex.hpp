#ifndef EVT_RTOS_MUTEX_
#define EVT_RTOS_MUTEX_

#include <core/rtos/Initializable.hpp>

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

    //Getters

    /**
      * Retrieves the name of this Mutex.
      *
      * @param[out] name a pointer to a place to store the name pointer.
      * @return The first error found by the function (or Success if there was no error).
      */
    TXError getName(char **name);

    /**
     * Retrieves the ownership count of this mutex. The ownership count is how many times the current
     * owner of this mutex has locked this mutex. Each time the owner unlocks this mutex, the ownership
     * count is decremented. Only when ownershipCount reaches 0 does the mutex actually unlock.
     *
     * @param[out] ownershipCount a pointer to a place to store the ownership count.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getOwnershipCount(uint32_t *ownershipCount);

    /**
     * Retrieves the name of the thread that currently owns this mutex.
     *
     * @param[out] name a pointer to a place to store the name of the owner of the thread.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getNameOfOwner(char **ownerName);

    /**
     * Retrieves the name of the first suspended thread.
     *
     * @param[out] threadName a pointer to a place to store the name of the first suspended thread.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getNameOfFirstSuspendedThread(char **threadName);

    /**
      * Retrieves the number of threads that are suspended on this Mutex.
      *
      * @param[out] numSuspendedThreads a pointer to a place to store the number of suspended threads.
      * @return The first error found by the function (or Success if there was no error).
      */
    TXError getNumSuspendedThreads(uint32_t *numSuspendedThreads);

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
