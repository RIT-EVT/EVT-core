#ifndef EVT_RTOS_THREAD_
#define EVT_RTOS_THREAD_

#include <core/rtos/Initializable.hpp>
#include <cstdint>

namespace core::rtos {

/**
 * Class that wraps a ThreadX Thread. Each thread represents a relatively independent task that will operate
 * "concurrently" with other threads. Each thread has its own stack, which is allocated in a BytePool.\n\n
 *
 * This class extends Initializable, and like all other Initializable classes must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 * @tparam T what type of data the thread's entry function will take. Should be at most 32 bits. (so generally should be
 * a pointer)
 */
template<typename T>
class Thread : public Initializable {
public:
    /**
     * Constructor for a Thread object. Thread will not start until init() method is called
     *
     * @param[in] name Pointer to a null-terminated character string representing the name of the thread
     * @param[in] entryFunction Pointer to the function the thread will be running
     * @param[in] data Pointer to the data the thread's function requires
     * @param[in] stackSize How much stack space (in bytes) this thread is allocated.
     * This must be greater than Threadx's minimum stack size, which is currently 200 bytes
     * @param[in] priority The initial priority of the thread (lower value = higher priority)
     * @param[in] preemptThreshold The value of the preemption threshold of the thread, which specifies what
     * priority of threads can interrupt this thread while running. This must be a higher priority than the
     * priority of the thread\n\n
     * A thread with a priority of 5 and a preemption threshold of 3, for instance, can only be interrupted by threads
     * with priorities 0,1,2, or 3. Without a preemption threshold, any thread with higher priority than the running
     * thread would be able to interrupt it.
     * @param[in] timeSlice How much time (in ticks) the thread will run before the scheduler may switch to another
     * thread
     * @param[in] autoStart Whether the thread starts as soon as it is initialized or is created suspended
     */
    Thread(char* name, void (*entryFunction)(T), T data, std::size_t stackSize, uint32_t priority,
           uint32_t preemptThreshold, uint32_t timeSlice, bool autoStart)
        : txThread(), name(name), entryFunction(entryFunction), data(data), stackSize(stackSize), priority(priority),
          preemptThreshold(preemptThreshold), timeSlice(timeSlice),
          autoStart(autoStart) {
    }

    /**
     * Thread Deconstructor
     */
    ~Thread() {
        tx_thread_terminate(&txThread);
        tx_thread_delete(&txThread);
    }

    /**
     * Create the threadx thread and possibly start it, depending on the autostart constructor parameter
     *
     * @return The first error found by the function or Success if there was no error
     */
    TXError init(BytePoolBase& pool) override {
        // allocate memory for the thread

        void* stackStart;
        uint32_t errorCode = pool.allocateMemory(stackSize, NO_WAIT, &stackStart);
        TXError error      = static_cast<TXError>(errorCode);
        if (error != SUCCESS)
            return error;
        // create the thread only if the memory allocation succeeded.
        errorCode = tx_thread_create(&txThread,
                                     name,
                                     (void (*)(ULONG)) entryFunction,
                                     (ULONG) data,
                                     stackStart,
                                     stackSize,
                                     priority,
                                     preemptThreshold,
                                     timeSlice,
                                     autoStart ? TX_AUTO_START : TX_DONT_START);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Register a function to be called when the thread initially is entered
     * and when the thread completes or is terminated
     *
     * @param[in] notifyFunction The pointer to the function that will be called.\n
     *  The first argument to this function will contain a pointer to this thread.\n
     *  The second argument to this function will be the threadID of this thread
     * @return The first error found by the function or Success if there was no error
     */
    TXError registerEntryExitNotification(void (*notifyFunction)(Thread<T>, uint32_t)) {
        // TODO: registerEntryExitNotification must be implemented
        return FEATURE_NOT_ENABLED;
    }

    /**
     * Set the preemptThreshold of the thread to the new values specified
     *
     * @param[in] newThreshold The new value that the thread's preemptThreshold will be set to
     * @param[out] oldThresholdOut A pointer to a space to store the previous value of the preemptThreshold
     * @return The first error found by the function or Success if there was no error
     */
    TXError setPreemptThreshold(uint32_t newThreshold, uint32_t* oldThresholdOut) {
        uint32_t errorCode = tx_thread_preemption_change(&txThread, newThreshold, (UINT*) oldThresholdOut);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Set the priority of the thread to the new value specified
     *
     * @param[in] newPriority The new value that the thread's priority will be set to
     * @param[out] oldPriorityOut A pointer to a space to store the previous value of the priority
     * @return The first error found by the function or Success if there was no error
     */
    TXError setPriority(uint32_t newPriority, uint32_t* oldPriorityOut) {
        uint32_t errorCode = tx_thread_priority_change(&txThread, newPriority, (UINT*) oldPriorityOut);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Set the timeSlice of the thread to the new value specified
     *
     * @param[in] newTimeSlice The new value that the thread's timeSlice will be set to
     * @param[out] oldTimeSliceOutput A pointer to a space to store the previous value of the timeSlice
     * @return The first error found by the function or Success if there was no error
     */
    TXError setTimeSlice(uint32_t newTimeSlice, uint32_t* oldTimeSliceOut) {
        uint32_t errorCode = tx_thread_time_slice_change(&txThread, newTimeSlice, oldTimeSliceOut);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Reset the thread so it is prepared to run the entry function again
     *
     * @return Enum representing The first error found by the function or Success if there was no error
     */
    TXError reset() {
        uint32_t errorCode = tx_thread_reset(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Resume the thread if it is suspended
     *
     * @return The first error found by the function or Success if there was no error
     */
    TXError resume() {
        uint32_t errorCode = tx_thread_resume(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Suspend the thread
     *
     * @return The first error found by the function or Success if there was no error
     */
    TXError suspend() {
        uint32_t errorCode = tx_thread_suspend(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Terminate the thread. Once the thread is terminated, it cannot be restarted
     *
     * @return The first error found by the function or Success if there was no error
     */
    TXError terminate() {
        uint32_t errorCode = tx_thread_terminate(&txThread);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Abort whatever wait this thread is in, returning WaitAborted to the method that this thread was waiting in
     *
     * @return The first error found by the function or Success if there was no error
     */
    TXError abortWait() {
        uint32_t errorCode = tx_thread_wait_abort(&txThread);
        return static_cast<TXError>(errorCode);
    }

    // Getters

    /**
     * Retrieve the name of this thread
     *
     * @param[out] name A pointer to a place to put the name pointer
     * @return The first error found by the function or Success if there was no error
     */
    TXError getName(char* const* name) {
        *name = this->name;
        return SUCCESS;
    }

    /**
     * Retrieve the state of this thread
     *
     * @param[out] state A pointer to a place to put the state of the thread
     * @return The first error found by the function or Success if there was no error
     */
    TXError getState(TXThreadState* state) {
        uint32_t errorCode =
            tx_thread_info_get(&txThread, nullptr, (UINT*) state, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Retrieve the run count of this thread
     *
     * @param[out] runCount A pointer to a place to put the run count
     * @return The first error found by the function or Success if there was no error
     */
    TXError getRunCount(uint32_t* runCount) {
        uint32_t errorCode =
            tx_thread_info_get(&txThread, nullptr, nullptr, runCount, nullptr, nullptr, nullptr, nullptr, nullptr);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Retrieve the priority of this thread
     *
     * @param priority A pointer to a place to put the priority.
     * @return The first error found by the function or Success if there was no error
     */
    TXError getPriority(uint32_t* priority) {
        uint32_t errorCode = tx_thread_info_get(
            &txThread, nullptr, nullptr, nullptr, (UINT*) priority, nullptr, nullptr, nullptr, nullptr);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Retrieve the preemption threshold of this thread
     *
     * @param[out] preemptThreshold A place to put the preempt threshold
     * @return The first error found by the function or Success if there was no error
     */
    TXError getPreemptThreshold(uint32_t* preemptThreshold) {
        uint32_t errorCode = tx_thread_info_get(
            &txThread, nullptr, nullptr, nullptr, nullptr, (UINT*) preemptThreshold, nullptr, nullptr, nullptr);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Retrieve the time slice of this thread
     *
     * @param[out] timeSlice A place to put the time slice
     * @return The first error found by the function or Success if there was no error
     */
    TXError getTimeSlice(uint32_t* timeSlice) {
        uint32_t errorCode =
            tx_thread_info_get(&txThread, nullptr, nullptr, nullptr, nullptr, nullptr, timeSlice, nullptr, nullptr);
        return static_cast<TXError>(errorCode);
    }

private:
    /** Threadx struct that holds the information for the thread */
    TX_THREAD txThread;

    /** Pointer to the name of this thread */
    char* name;

    /** The function this thread will be running */
    void (*entryFunction)(T);

    /** The data the thread function requires */
    T data;

    /** How much stack space the thread requires */
    std::size_t stackSize;

    /** The priority rating of this thread */
    uint32_t priority;

    /** The preemption rating of this thread */
    uint32_t preemptThreshold;

    /** The time slice of this thread */
    uint32_t timeSlice;

    /** Whether this thread will start when initialized or not */
    bool autoStart;
};

} // namespace core::rtos

#endif // EVT_RTOS_THREAD_
