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
     * Set the eventFlags defined by the mask.
     * Equivalent to mask | eventFlags, i.e. a bitwise OR with mask.
     * Does not affect bits not included in the mask
     *
     * @param[in] mask The bitmask of flags to set
     *
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError set(uint32_t mask);

    /**
     * Clears the eventFlags defined by the mask.
     * Equivalent to ~mask & eventFlags, i.e. a bitwise AND with the negation of the mask.
     * Does not affect bits not included in the mask
     *
     * @param[in] mask The bitmask of flags to clear
     *
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError clear(uint32_t mask);

    /**
     * Get the eventFlags defined by the mask
     *
     * @param[in] mask A bitmask describing what flags are requested
     * @param[in] waitForAllFlags If the method should wait for EVERY flag in the bitmask
     * to be true, or just ANY flag in the bitmask to be true
     * @param[in] clear If the method should clear outputted flags
     * @param[in] waitOption How long (in ticks) the calling thread should wait for this method to
     * return. Use Enums::TXWait::WaitForever to wait forever
     * @param[out] output The returned flags
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError get(uint32_t mask, bool waitForAllFlags, bool clear, uint32_t waitOption, uint32_t* output);

    /**
     * Register a function that will be called whenever an event flag in this group is set
     *
     * @param[in] notifyFunction The function that will be called whenever an event flag in the group is set
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError registerNotifyFunction(void (*notifyFunction)(EventFlags* eventFlags));

    /**
     * Get the name of this EventFlags
     *
     * @param[out] name The returned name
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getName(char** name);

    /**
     * Get the current values that the flags are set to as a uint32_t
     *
     * @param[out] flags The returned flags
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getCurrentFlags(uint32_t* flags);

    /**
     * Get the name of the first suspended thread
     *
     * @param[out] name The returned name
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getNameOfFirstSuspendedThread(char** threadName);

    /**
     * Get the number of threads that are suspended on this EventFlags
     *
     * @param[out] numSuspendedThreads The returned number of suspended threads
     * @return The first error found by the function or TXE_SUCCESS if there was no error.
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
