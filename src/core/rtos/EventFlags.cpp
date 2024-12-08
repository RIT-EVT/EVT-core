#include <core/rtos/EventFlags.hpp>

namespace core::rtos {

EventFlags::EventFlags(char* name) : Initializable(name), txEventFlagsGroup() {}

TXError EventFlags::init(core::rtos::BytePoolBase& pool) {
    return static_cast<TXError>(tx_event_flags_create(&txEventFlagsGroup, name));
}

EventFlags::~EventFlags() {
    tx_event_flags_delete(&txEventFlagsGroup);
}

TXError EventFlags::set(uint32_t mask) {
    return static_cast<TXError>(tx_event_flags_set(&txEventFlagsGroup, mask, TX_OR));
}

TXError EventFlags::clear(uint32_t mask) {
    return static_cast<TXError>(tx_event_flags_set(&txEventFlagsGroup, ~mask, TX_AND));
}

TXError EventFlags::get(uint32_t mask, bool waitForAllFlags, bool clear, uint32_t waitOption, uint32_t* output) {
    // ThreadX packs waitForAllFlags and clear into one uint32_t option where the first bit is whether to clear,
    //  and the second bit is whether to wait.
    uint32_t option = ((uint32_t) waitForAllFlags << 1) | ((uint32_t) clear);
    return static_cast<TXError>(tx_event_flags_get(&txEventFlagsGroup, mask, option, output, waitOption));
}

TXError EventFlags::registerNotifyFunction(void (*notifyFunction)(EventFlags*)) {
    // todo: registerNotifyFunction must be implemented
    return TXE_FEATURE_NOT_ENABLED;
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
    if (status != TXE_SUCCESS) {
        return static_cast<TXError>(status);
    }

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
