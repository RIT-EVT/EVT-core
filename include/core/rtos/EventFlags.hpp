#ifndef _EVT_RTOS_EVENTFLAGS_
#define _EVT_RTOS_EVENTFLAGS_

#include <core/rtos/Initializable.hpp>

namespace core::rtos {

/**
 * Class that wraps a ThreadX Event Flags Group. Event flags are essentially
 * a 32-bit mask, with each bit representing a different flag.\n\n
 *
 * This class extends Initializable, and like all other Initializable classes must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 */
class EventFlags : public Initializable {
public:
    /**
     * Constructor for EventFlags.
     *
     * @param[in] name the name of the EventFlags.
     */
    EventFlags(char* name);

    TXError init(core::rtos::BytePoolBase& pool) override;

    /**
     * Destructor for an EventFlags.
     */
    ~EventFlags();

    /**
     * Sets the eventFlags of the mask according to the setOption.
     *
     * @param[in] mask A bitmask describing what flags are to be interacted with.
     * @param[in] clearNonMaskedFlags If true, all the flags not included in the mask will be set to 0.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError set(uint32_t mask, bool clearNonMaskedFlags);

    /**
     * Gets the eventFlags of the mask according to the getOption.
     *
     * @param[in] mask A bitmask describing what flags are requested.
     * @param[in] waitForAllFlags If the method should wait for EVERY flag in the bitmask
     * to be true, or just ANY flag in the bitmask to be true.
     * @param[in] clear If the method should clear outputted flags.
     * @param[in] waitOption How long (in ticks) the calling thread should wait for this method to
     * return. Use Enums::TXWait::WaitForever to wait forever.
     * @param[out] output A pointer to store the set flags in.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError get(uint32_t mask, bool waitForAllFlags, bool clear, uint32_t waitOption, uint32_t* output);

    /**
     * Registers a function that will be called whenever an event flag in this group is set.
     *
     * @param[in] notifyFunction The function that will be called whenever an event flag in the group is set.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError registerNotifyFunction(void (*notifyFunction)(EventFlags* eventFlags));

    /**
     * Retrieves the name of this EvenFlags.
     *
     * @param[out] name A pointer to a place to store the name pointer.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getName(char** name);

    /**
     * Retrieves the current values that the flags are set to as a uint32.
     *
     * @param[out] flags A pointer to a place to store the flags.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getCurrentFlags(uint32_t* flags);

    /**
     * Retrieves the name of the first suspended thread.
     *
     * @param[out] name A pointer to a place to store the name pointer.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Retrieves the number of threads that are suspended on this EventFlags.
     *
     * @param[out] numSuspendedThreads A pointer to a place to store the number of suspended threads.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads);

private:
    /**
     * The name of this object.
     */
    char* name;

    /**
     * The threadx struct that represents this object in the threadx kernel.
     */
    TX_EVENT_FLAGS_GROUP txEventFlagsGroup;

    /**
     * The type of notify function that threadx expects.
     */
    typedef void txNotifyFunction_t(TX_EVENT_FLAGS_GROUP*);

    /**
     * A pointer to the function that we will register with the threadx kernel when the
     * registerNotificationFunction method is called. This function calls memberNotifyFunction, which itself calls
     * storedNotifyFunction, which will be set to the passed-in function for the registerNotifyFunction method.
     */
    txNotifyFunction_t* txNotifyFunction;
};

} // namespace core::rtos

#endif //_EVT_RTOS_EVENTFLAGS_
