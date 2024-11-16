#ifndef EVT_RTOS_SEMAPHORE_
#define EVT_RTOS_SEMAPHORE_

#include <core/rtos/Enums.hpp>
#include <core/rtos/Initializable.hpp>

namespace core::rtos {

/**
 * Class that wraps a ThreadX Semaphore.\n\n
 *
 * This class extends Initializable, and like all other Initializable classes must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 */
class Semaphore : public Initializable {
public:
    /**
     * Construct a Semaphore object, but does not initialize it (must call init before using)
     *
     * @param[in] name The name of the Semaphore
     * @param[in] initialCount What number the Semaphore will start at
     */
    Semaphore(char* name, uint32_t initialCount);

    /**
     * Semaphore Destructor
     */
    ~Semaphore();

    TXError init(BytePoolBase& pool) override;

    /**
     * Get an instance of the semaphore. If there is no instance to get, the calling thread will suspend for
     * waitOption ticks for the semaphore to be provided an instance
     *
     * @param[in] waitOption How long (in ticks) the calling thread should wait for the mutex to be available.
     * Use TXW_WAIT_FOREVER to wait forever
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError get(uint32_t waitOption);

    /**
     * Put an instance of the semaphore
     *
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError put();

    /**
     * Put an instance of the semaphore, up to the provided ceiling
     *
     * @param[in] ceiling the max value we want this command to be able to put the semaphore to
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError putWithCeiling(uint32_t ceiling);

    /**
     * Register a function to be called whenever this Semaphore is put
     *
     * @param notifyFunction The function to be called when a the semaphore is put
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError registerNotifyFunction(void (*notifyFunction)(Semaphore*));

    /**
     * Prioritize the list of waiting threads, placing the highest priority waiting thread at the front of the list
     * (Does not effect the order of the other waiting threads)
     *
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError prioritize();

    /**
     * Get the name of this Semaphore
     *
     * @param[out] name The returned name
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getName(char** name);

    /**
     * Get the current count of this Semaphore
     *
     * @param currentCount The returned count
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getCount(uint32_t* currentCount);

    /**
     * Get the name of the first suspended thread
     *
     * @param[out] threadName The returned name of the first suspended thread
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Get the number of threads that are suspended on this Semaphore
     *
     * @param[out] numSuspendedThreads The returned number of suspended threads
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads);

private:
    /** The name of the Semaphore */
    char* name;

    /** Threadx struct that actually holds all of the information for the Semaphore */
    TX_SEMAPHORE txSemaphore;

    /** The count the Semaphore will be initialized with */
    const uint32_t initialCount;
};

} // namespace core::rtos

#endif // EVT_RTOS_SEMAPHORE_
