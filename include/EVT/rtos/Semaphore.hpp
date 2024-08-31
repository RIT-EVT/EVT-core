

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

    /**
     * Register a function to be called whenever this Semaphore is put.
     *
     * @param notifyFunction The function to be called when a message is sent to the queue.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError registerNotifyFunction(void(*notifyFunction)(Semaphore*));

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

    void(*storedNotifyFunction)(Semaphore*);

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
