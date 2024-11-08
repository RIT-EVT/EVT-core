#ifndef _EVT_RTOS_EVENTFLAGS_
#define _EVT_RTOS_EVENTFLAGS_

#include <core/rtos/Initializable.hpp>

namespace core::rtos {

/**
 * Class that wraps a ThreadX Event Flags Group. Event Flags are essentially
 * a 32-bit unsigned integer, with each bit representing a different flag.
 * \n\n
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

    /**
     * Destructor for EventFlags.
     */
    ~EventFlags();

    TXError init(core::rtos::BytePoolBase& pool) override;

    /**
     * Set the eventFlags defined by the mask
     *
     * @param[in] mask A bitmask describing what flags are to be interacted with
     * @param[in] clearNonMaskedFlags If true, all the flags not included in the mask will be set to 0.
     * (true is equivalent to AND-ing with the mask, false is equivalent to OR-ing with the mask)
     * @return The first error found by the function or Success if there was no error
     */
    TXError set(uint32_t mask, bool clearNonMaskedFlags);

    /**
     * Get the eventFlags defined by the mask
     *
     * @param[in] mask A bitmask describing what flags are requested
     * @param[in] waitForAllFlags If the method should wait for EVERY flag in the bitmask
     * to be true, or just ANY flag in the bitmask to be true
     * @param[in] clear If the method should clear outputted flags
     * @param[in] waitOption How long (in ticks) the calling thread should wait for this method to
     * return. Use Enums::TXWait::WaitForever to wait forever
     * @param[out] output A pointer to store the set flags in
     * @return The first error found by the function or Success if there was no error
     */
    TXError get(uint32_t mask, bool waitForAllFlags, bool clear, uint32_t waitOption, uint32_t* output);

    /**
     * Register a function that will be called whenever an event flag in this group is set
     *
     * @param[in] notifyFunction The function that will be called whenever an event flag in the group is set
     * @return The first error found by the function or Success if there was no error
     */
    TXError registerNotifyFunction(void (*notifyFunction)(EventFlags* eventFlags));

    /**
     * Retrieve the name of this EventFlags
     *
     * @param[out] name A pointer to a place to store the name pointer
     * @return The first error found by the function or Success if there was no error
     */
    TXError getName(char** name);

    /**
     * Retrieve the current values that the flags are set to as a uint32_t
     *
     * @param[out] flags A pointer to a place to store the flags
     * @return The first error found by the function or Success if there was no error
     */
    TXError getCurrentFlags(uint32_t* flags);

    /**
     * Retrieve the name of the first suspended thread
     *
     * @param[out] name A pointer to a place to store the name pointer
     * @return The first error found by the function or Success if there was no error
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Retrieve the number of threads that are suspended on this EventFlags
     *
     * @param[out] numSuspendedThreads A pointer to a place to store the number of suspended threads
     * @return The first error found by the function or Success if there was no error.
     */
    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads);

private:
    /** The name of this object */
    char* name;

    /** The threadx struct that represents this object in the threadx kernel */
    TX_EVENT_FLAGS_GROUP txEventFlagsGroup;
};

} // namespace core::rtos

#endif //_EVT_RTOS_EVENTFLAGS_
