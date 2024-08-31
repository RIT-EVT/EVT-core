
#include <EVT/rtos/EventFlags.hpp>
#include <functional>

namespace core::rtos {

EventFlags::EventFlags(char* name) : name(name), txEventFlagsGroup() {
    //We use bind to return a callable object that takes in only one argument, functionally removing the
    //implicit first argument that the memberNotifyFunction has.
    auto boundFunc = std::bind(&EventFlags::memberNotifyFunction, this, std::placeholders::_1);
    //We wrap this callable object into a wrapFunc so we can use .target on it.
    std::function<void(TX_EVENT_FLAGS_GROUP*)> wrapFunc = boundFunc;
    //We use the .target method to return a c-style function pointer that we can later pass to threadx
    //in the event that registerNotifyFunction is called.
    txNotifyFunction = wrapFunc.target<txNotifyFunction_t>();
}

TXError EventFlags::init(core::rtos::BytePoolBase& pool) {
    return static_cast<TXError>(tx_event_flags_create(&txEventFlagsGroup, name));
}

EventFlags::~EventFlags() {
   tx_event_flags_delete(&txEventFlagsGroup);
}

TXError EventFlags::set(uint32_t mask, bool clearNonMaskedFlags) {
    return static_cast<TXError>(tx_event_flags_set(&txEventFlagsGroup, mask, clearNonMaskedFlags ? TX_AND : TX_OR));
}

TXError EventFlags::get(uint32_t mask, bool waitForAllFlags, bool clear, uint32_t* output, uint32_t waitOption) {
    uint32_t option = waitForAllFlags ? TX_AND : TX_OR;
    //the TX_AND_CLEAR and TX_AND_OR options are respectively 1 more than their no clear versions, which is why this works.
    option += clear;
    return static_cast<TXError>(tx_event_flags_get(&txEventFlagsGroup, mask, option, output, waitOption));
}

TXError EventFlags::registerNotifyFunction(void (*notifyFunction)(core::rtos::EventFlags*)) {
    storedNotifyFunction = notifyFunction;
    return static_cast<TXError>(tx_event_flags_set_notify(&txEventFlagsGroup, txNotifyFunction));
}

} //namespace core::rtos
