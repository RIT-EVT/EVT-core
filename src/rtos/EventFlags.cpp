
#include <EVT/rtos/EventFlags.hpp>

namespace core::rtos {

EventFlags::EventFlags(char* name) : name(name), txEventFlagsGroup() {}

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
