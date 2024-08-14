
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



} //namespace core::rtos
