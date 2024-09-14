
#include <EVT/rtos/Thread.hpp>
#include <functional>

namespace core::rtos {

template<typename T>
Thread<T>::Thread(const char* name, void (*entryFunction)(T*), T* data, std::size_t stackSize,
                  uint32_t priority, uint32_t preemptThreshold, uint32_t timeSlice, bool autoStart)
    : name(name), entryFunction(entryFunction), data(data), stackSize(stackSize), priority(priority),
        preemptThreshold(preemptThreshold), timeSlice(timeSlice), autoStart(autoStart) {
    //We use bind to return a callable object that takes in only one argument, functionally removing the
    //implicit first argument that the memberNotifyFunction has.
    auto boundFunc = std::bind(&Thread<T>::memberNotifyFunction, this, std::placeholders::_1, std::placeholders::_2);
    //We wrap this callable object into a wrapFunc so we can use .target on it.
    std::function<void(TX_QUEUE*)> wrapFunc = boundFunc;
    //We use the .target method to return a c-style function pointer that we can later pass to threadx
    //in the event that registerNotifyFunction is called.
    txNotifyFunction = wrapFunc.target<txNotifyFunction_t>();
}

template<typename T>
Thread<T>::~Thread() {
    tx_thread_terminate(&txThread);
    tx_thread_delete(&txThread);
}

template<typename T>
TXError Thread<T>::init(core::rtos::BytePoolBase& pool) {
    //allocate memory for the thread
    void *stackStart;
    uint32_t errorCode = pool.allocateMemory(stackSize, &stackStart, NoWait);
    TXError error = static_cast<TXError>(errorCode);
    if (error != Success) return error;
    //create the thread only if the memory allocation succeeded.
    errorCode = tx_thread_create(&txThread, name, entryFunction, data, stackStart,
                     stackSize, priority, preemptThreshold, timeSlice, autoStart ? TX_AUTO_START : TX_DONT_START);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::registerEntryExitNotification(void(*notifyFunction)(Thread<T>, uint32_t)) {
    storedNotifyFunction = notifyFunction;
    uint32_t errorCode = tx_thread_entry_exit_notify(&txThread, txNotifyFunction);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::setPreemptThreshold(uint32_t newThreshold, uint32_t* oldThresholdOut) {
    uint32_t errorCode = tx_thread_preemption_change(&txThread, newThreshold, (UINT*)oldThresholdOut);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::setPriority(uint32_t newPriority, uint32_t* oldPriorityOut) {
    uint32_t errorCode = tx_thread_priority_change(&txThread, newPriority, (UINT*)oldPriorityOut);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::setTimeSlice(uint32_t newTimeSlice, uint32_t* oldTimeSliceOut) {
    uint32_t errorCode = tx_thread_time_slice_change(&txThread, newTimeSlice, oldTimeSliceOut);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::reset() {
    uint32_t errorCode = tx_thread_reset(&txThread);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::resume() {
    uint32_t errorCode = tx_thread_resume(&txThread);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::suspend() {
    uint32_t errorCode = tx_thread_suspend(&txThread);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::terminate() {
    uint32_t errorCode = tx_thread_terminate(&txThread);
    return static_cast<TXError>(errorCode);
}

template<typename T>
TXError Thread<T>::abortWait() {
    uint32_t errorCode = tx_thread_wait_abort(&txThread);
    return static_cast<TXError>(errorCode);
}

} //namespace core::rtos
