#include <core/rtos/EventFlags.hpp>

namespace core::rtos {

EventFlags::EventFlags(char* name) : name(name), txEventFlagsGroup(){}

TXError EventFlags::init(core::rtos::BytePoolBase& pool) {
    return static_cast<TXError>(tx_event_flags_create(&txEventFlagsGroup, name));
}

EventFlags::~EventFlags() {
    tx_event_flags_delete(&txEventFlagsGroup);
}

TXError EventFlags::set(uint32_t mask, bool clearNonMaskedFlags) {
    return static_cast<TXError>(tx_event_flags_set(&txEventFlagsGroup, mask, clearNonMaskedFlags ? TX_AND : TX_OR));
}

TXError EventFlags::get(uint32_t mask, bool waitForAllFlags, bool clear, uint32_t waitOption, uint32_t* output) {
    uint32_t option = waitForAllFlags ? TX_AND : TX_OR;
    // the TX_AND_CLEAR and TX_AND_OR options are respectively 1 more than their no clear versions, which is why this
    // works.
    option += clear;
    return static_cast<TXError>(tx_event_flags_get(&txEventFlagsGroup, mask, option, output, waitOption));
}

TXError EventFlags::registerNotifyFunction(void (*notifyFunction)(EventFlags*)) {
    //todo: registerNotifyFunction must be implemented
    return FEATURE_NOT_ENABLED;
}

TXError EventFlags::getName(char** name) {
    *name = (this->name);
    return SUCCESS;
}

TXError EventFlags::getCurrentFlags(uint32_t* flags) {
    uint32_t status = tx_event_flags_info_get(&txEventFlagsGroup, nullptr, flags, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

TXError EventFlags::getNameOfFirstSuspendedThread(char** threadName) {
    // grab a pointer to the first suspended thread struct
    TX_THREAD* thread;
    uint32_t status = tx_event_flags_info_get(&txEventFlagsGroup, nullptr, nullptr, &thread, nullptr, nullptr);

    // exit early if the call failed
    if (status != SUCCESS)
        return static_cast<TXError>(status);

    // read the name off the struct
    status = tx_thread_info_get(thread, threadName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    return static_cast<TXError>(status);
}

TXError EventFlags::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    uint32_t status =
        tx_event_flags_info_get(&txEventFlagsGroup, nullptr, nullptr, nullptr, numSuspendedThreads, nullptr);
    return static_cast<TXError>(status);
}

} // namespace core::rtos
