

#ifndef EVT_RTOS_SEMAPHORE_
#define EVT_RTOS_SEMAPHORE_

#include <EVT/rtos/Initializable.hpp>

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
     * Constructs a Semaphore object, but does not initialize it (must call init before using).
     *
     * @param[in] name The name of the Semaphore.
     * @param[in] initialCount What number the Semaphore will start at.
     */
    Semaphore(char* name, uint32_t initialCount);

    TXError init(BytePoolBase& pool) override;

    /**
     * Semaphore Destructor.
     */
    ~Semaphore();

    /**
     * Gets an instance of the semaphore. If there is no instance to get, the calling thread will suspend for
     * waitOption ticks for the semaphore to be provided an instance.
     *
     * @param[in] waitOption How long (in ticks) the calling thread should wait for the mutex to be available.
     * Use Enums::TXWait::WaitForever to wait forever
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError get(uint32_t waitOption);

    /**
     * Puts an instance of the semaphore.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError put();

    /**
     * Puts an instance of the semaphore, up to the provided ceiling.
     *
     * @param[in] ceiling the max value we want this command to be able to put the semaphore to.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError ceilingPut(uint32_t ceiling);

    /**
     * Register a function to be called whenever this Semaphore is put.
     *
     * @param notifyFunction The function to be called when a message is sent to the queue.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError registerNotifyFunction(void(*notifyFunction)(Semaphore*));

    /**
     * Prioritizes the list of waiting threads, placing the highest priority waiting thread at the front of the list.
     * (Does not effect the order of the other waiting threads).
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError prioritize();

private:
    /**
     * The name of the Semaphore.
     */
    char* name;

    /**
     * Threadx struct that actually holds all of the information for the Semaphore.
     */
    TX_SEMAPHORE txSemaphore;

    /**
     * The count the Semaphore will be initialized with.
     */
    const uint32_t initialCount;

    /**
     * The notify function that has been registered with this event flag.
     */
    void(*storedNotifyFunction)(Semaphore*);

    /**
     * The notification function that we would like threadx to call.
     * Unfortunately, threadx cannot actually call this function because member functions implicitly
     * have an extra argument for the object that the member function is being called on.
     * So, in the constructor we do some weird c++ things with std::bind and std::function in
     * order to create a non-member function that threadx can call, which is txNotifyFunction.
     */
    void memberNotifyFunction(TX_SEMAPHORE * queue) {
        storedNotifyFunction(this);
    }

    /**
     * The type of notify function that threadx expects.
     */
    typedef void txNotifyFunction_t( TX_SEMAPHORE * );

    /**
     * A pointer to the function that we will register with the threadx kernel when the
     * registerNotificationFunction method is called. This function calls memberNotifyFunction, which itself calls
     * storedNotifyFunction, which will be set to the passed-in function for the registerNotifyFunction method.
     */
    txNotifyFunction_t *txNotifyFunction;
};

} // namespace core::rtos

#endif//EVT_RTOS_SEMAPHORE_
