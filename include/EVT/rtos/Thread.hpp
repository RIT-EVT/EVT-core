
#ifndef EVT_RTOS_THREAD_
#define EVT_RTOS_THREAD_

#include <EVT/rtos/Initializable.hpp>
#include <cstdint>
#include <functional>

namespace core::rtos {

/**
 * Class that wraps a ThreadX Thread. Each thread represents a relatively independent task that will operate
 * "concurrently" with other threads. Each thread has its own stack, which is allocated in a BytePool.\n\n
 *
 * This class extends Initializable, and like all other Initializable classes must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 * @tparam T what type of data the thread's entry function will take.
 */
template <typename T>
class Thread : public Initializable {
public:

    /**
     * Constructor for a Thread object. Thread will not start until init() method is called.
     *
     * @param[in] name Pointer to a null-terminated character string representing the name of the thread.
     * @param[in] entryFunction Pointer to the function the thread will be running.
     * @param[in] data Pointer to the data the thread's function requires.
     * @param[in] stackSize How much stack space (in bytes) this thread is allocated.
     * @param[in] priority The initial priority of the thread (lower value = higher priority).
     * @param[in] preemptThreshold The value of the preemption threshold of the thread, which specifies what
     * priority of threads can interrupt this thread while running. \n\n
     * A thread with a priority of 5 and a preemption threshold of 3, for instance, can only be interrupted by threads
     * with priorities 0,1,2, or 3. Without a preemption threshold, any thread with higher priority than the running
     * thread would be able to interrupt it.
     * @param[in] timeSlice How much time (in ticks) the thread will run before the scheduler may switch to another thread.
     * @param[in] autoStart Whether the thread starts as soon as it is initialized or is created suspended.
     */
    Thread(char* name, void (*entryFunction)(T), T data, std::size_t stackSize, uint32_t priority,
           uint32_t preemptThreshold, uint32_t timeSlice, bool autoStart)
        : name(name), entryFunction(entryFunction), data(data), stackSize(stackSize), priority(priority),
          preemptThreshold(preemptThreshold), timeSlice(timeSlice), autoStart(autoStart), txNotifyFunction(txThreadNotifyFunctionTemplate<this>) {

    }

    /**
     * Creates the threadx thread and possibly starts it, depending on the autostart constructor parameter.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError init(BytePoolBase &pool) override {
        //allocate memory for the thread
        void *stackStart;
        uint32_t errorCode = pool.allocateMemory(stackSize, &stackStart, NoWait);
        TXError error = static_cast<TXError>(errorCode);
        if (error != Success) return error;
        //create the thread only if the memory allocation succeeded.
        errorCode = tx_thread_create(&txThread, name, (void (*)(ULONG))entryFunction, (ULONG)data, stackStart,
                                     stackSize, priority,preemptThreshold, timeSlice,
                                     autoStart ? TX_AUTO_START : TX_DONT_START);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Thread Deconstructor
     */
    ~Thread() {
        tx_thread_terminate(&txThread);
        tx_thread_delete(&txThread);
    }

    /**
     * Registers a function to be called when the thread initially is entered
     * and when the thread completes or is terminated.
     *
     * @param[in] notifyFunction The pointer to the function that will be called.\n
     *  The first argument to this function will contain a pointer to this thread.\n
     *  The second argument to this function will be the threadID of this thread.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError registerEntryExitNotification(void(*notifyFunction)(Thread<T>, uint32_t)) {
        storedThreadNotifyFunction = reinterpret_cast<void(*)(Initializable*, uint32_t)>(notifyFunction);
        uint32_t errorCode = tx_thread_entry_exit_notify(&txThread, txNotifyFunction);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Sets the preemptThreshold of the thread to the new values specified.
     *
     * @param[in] newThreshold The new value that the thread's preemptThreshold will be set to.
     * @param[out] oldThresholdOut A pointer to a space to store the previous value of the preemptThreshold
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError setPreemptThreshold(uint32_t newThreshold, uint32_t* oldThresholdOut) {
        uint32_t errorCode = tx_thread_preemption_change(&txThread, newThreshold, (UINT*)oldThresholdOut);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Sets the priority of the thread to the new value specified.
     *
     * @param[in] newPriority The new value that the thread's priority will be set to.
     * @param[out] oldPriorityOut A pointer to a space to store the previous value of the priority.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError setPriority(uint32_t newPriority, uint32_t* oldPriorityOut) {
        uint32_t errorCode = tx_thread_priority_change(&txThread, newPriority, (UINT*)oldPriorityOut);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Sets the timeSlice of the thread to the new value specified.
     *
     * @param[in] newTimeSlice The new value that the thread's timeSlice will be set to.
     * @param[out] oldTimeSliceOutput A pointer to a space to store the previous value of the timeSlice.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError setTimeSlice(uint32_t newTimeSlice, uint32_t* oldTimeSliceOut) {
        uint32_t errorCode = tx_thread_time_slice_change(&txThread, newTimeSlice, oldTimeSliceOut);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Resets the thread so it is prepared to run the entry function again.
     *
     * @return Enum representing the first error found by the function (or Success if there was no error).
     */
    TXError reset() {
        uint32_t errorCode = tx_thread_reset(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Resumes the thread if it is suspended.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError resume() {
        uint32_t errorCode = tx_thread_resume(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Suspends the thread.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError suspend() {
        uint32_t errorCode = tx_thread_suspend(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Terminates the thread. Once this thread is terminated, it cannot be restarted.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError terminate() {
        uint32_t errorCode = tx_thread_terminate(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Aborts whatever wait this thread is in, returning WaitAborted to the method that this thread was waiting in.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError abortWait() {
        uint32_t errorCode = tx_thread_wait_abort(&txThread);
        return static_cast<TXError>(errorCode);
    }

private:
    /**
     * Threadx struct that holds the information for the thread.
     */
    TX_THREAD txThread;

    /**
     * Pointer to the name of this thread.
     */
    char* name;

    /**
     * The function this thread will be running.
     */
    void (*entryFunction)(T);

    /**
     * The data the thread function requires.
     */
    T data;

    /**
     * How much stack space the thread requires.
     */
    std::size_t stackSize;

    /**
     * The priority rating of this thread.
     */
    uint32_t priority;

    /**
     * The preemption rating of this thread.
     */
    uint32_t preemptThreshold;

    /**
     * The time slice of this thread.
     */
    uint32_t timeSlice;

    /**
     * Whether this thread will start when initialized or not.
     */
    bool autoStart;

    /**
     * Place to hold the template txNotifyFunctionTemplate
     */
    void(*txNotifyFunction)(TX_THREAD*, UINT);
};

} // namespace core::rtos

#endif//EVT_RTOS_THREAD_
