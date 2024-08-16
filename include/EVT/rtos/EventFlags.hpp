
#ifndef _EVT_RTOS_EVENTFLAGS
#define _EVT_RTOS_EVENTFLAGS

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class EventFlags : Initializable {
public:

    /**
     * Constructor for an EventFlags.
     *
     * @param name the name of the EventFlags.
     */
    EventFlags(const char* name);

    TXError init(core::rtos::BytePoolBase &pool) override;

    /**
     * Destructor for an EventFlags.
     */
    ~EventFlags();

    /**
     * Sets the eventFlags of the mask according to the setOption.
     *
     * @param[in] mask A bitmask describing what flags are to be interacted with.
     * @param[in] setOption How the flags are to be set: TRUE = And, FALSE = Or.
     *
     * (This means that if setOption is true, the eventFlags will be simply set to the mask.
     * If setOption is false, the current eventFlags and the mask will be binary or-ed together.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError set(uint32_t mask, bool setOption);

    /**
     * Gets the eventFlags of the mask according to the getOption
     *
     * @param mask A bitmask describing what flags are requested.
     * @param waitforAllFlags If the method should wait for EVERY flag in the bitmask
     * to be true, or just ANY flag in the bitmask to be true.
     * @param[in] clear If the method should clear outputted flags.
     * @param output A pointer a place to store the outputted flags.
     * @param waitOption How long (in ticks) the calling thread should wait for this method to
     * return, or Enums::TXWait::WaitForever to wait forever.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError get(uint32_t mask, bool waitForAllFlags, bool clear, uint32_t* output, uint32_t waitOption);

    /**
     * Registers a function that will be called whenever an event flag in this group is set.
     *
     * @param notifyFunction The function that will be called whenever an event flag in the group is set.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError registerNotifyFunction(void (*notifyFunction)(EventFlags* eventFlags));

private:
    /**
     * The name of this object.
     */
    const char * name;

    /**
     * The threadx struct that represents this object in the threadx kernel.
     */
    TX_EVENT_FLAGS_GROUP txEventFlagsGroup;

    /**
     * The notify function that has been registered with this event flag.
     */
    void (*storedNotifyFunction)(EventFlags* eventFlags);

    /**
     * The actual notify function that we will register with the threadx kernel
     */
    void txNotifyFunction(TX_EVENT_FLAGS_GROUP* txEventFlagsGroup) {
        storedNotifyFunction(this);
    }
};

} //namespace core::rtos

#endif _EVT_RTOS_EVENTFLAGS
