
#include <EVT/rtos/Thread.hpp>

namespace core::rtos {

template<typename T>
Thread<T>::Thread(const char* name, void (*entryFunction)(T*), T* data, std::size_t stackSize,
                  uint32_t priority, uint32_t preemptThreshold, uint32_t timeSlice, bool autoStart)
    : name(name), entryFunction(entryFunction), data(data), stackSize(stackSize), priority(priority),
        preemptThreshold(preemptThreshold), timeSlice(timeSlice), autoStart(autoStart) {}


/**
 * TODO: should the deconstructor call terminate() before calling delete?
 * Otherwise if the thread is not finished, the delete function will not actually work
 */
template<typename T>
Thread<T>::~Thread() {
    tx_thread_delete(&txThread);
}

template<typename T>
TXError Thread<T>::init(core::rtos::BytePoolBase& pool) {
    void *stackStart;
    stackStart = pool.AllocateMemory(stackSize, 0);
    tx_thread_create(&txThread, name, entryFunction, data, stackStart,
                     stackSize, priority, preemptThreshold, timeSlice, autoStart ? TX_AUTO_START : TX_DONT_START);
}

template<typename T>
TXError Thread<T>::registerEntryExitNotification(threadNotifyFunction_t notifyFunction) {
    tx_thread_entry_exit_notify(&txThread, notifyFunction);
}

template<typename T>
TXError Thread<T>::setPreemptThreshold(uint32_t newThreshold, uint32_t* oldThresholdOut) {
    preemptThreshold = newThreshold;
    tx_thread_preemption_change(&txThread, newThreshold, oldThresholdOut);
}

template<typename T>
TXError Thread<T>::setPriority(uint32_t newPriority, uint32_t* oldPriorityOut) {
    priority = newPriority;
    tx_thread_priority_change(&txThread, newPriority, oldPriorityOut);
}

template<typename T>
TXError Thread<T>::setTimeSlice(uint32_t newTimeSlice, uint32_t* oldTimeSliceOut) {
    timeSlice = newTimeSlice;
    tx_thread_time_slice_change(&txThread, newTimeSlice, oldTimeSliceOut);
}

template<typename T>
TXError Thread<T>::reset() {
    tx_thread_reset(&txThread);
}

template<typename T>
TXError Thread<T>::resume() {
    tx_thread_resume(&txThread);
}

template<typename T>
TXError Thread<T>::suspend() {
    tx_thread_suspend(&txThread);
}

template<typename T>
TXError Thread<T>::terminate() {
    tx_thread_terminate(&txThread);
}

template<typename T>
TXError Thread<T>::abortWait() {
    tx_thread_wait_abort(&txThread)
}

} //namespace core::rtos
