
#ifndef EVT_RTOS_THREAD_
#define EVT_RTOS_THREAD_

#include <EVT/rtos/Initializable.hpp>
#include <cstdint>


namespace core::rtos {

template <typename T>
class Thread : Initializable {
public:

    /**
     * Constructor for a Thread object. Thread will not start until init() method is called.
     *
     * @param[in] name pointer to a null-terminated character string representing the name of the thread
     * @param[in] entryFunction Function the thread will be running.
     * @param[in] data Data the thread's function requires.
     * @param[in] stackSize How much stack space this thread is allocated.
     * @param[in] priority The initial priority of the thread (lower value = higher priority).
     * @param[in] preemptThreshold The value of the preemption threshold of the thread.
     * @param[in] timeSlice How much time the thread will run before the scheduler may switch to another thread.
     * @param[in] autoStart Whether the thread starts as soon as it is initialized or is created suspended.
     */
    Thread(const char* name, void (*entryFunction)(T*), T* data, std::size_t stackSize, uint32_t priority,
           uint32_t preemptThreshold, uint32_t timeSlice, bool autoStart);

    /**
     * Creates the threadx thread and possibly starts it, depending on the autostart constructor parameter.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError init(BytePoolBase &pool) override;

    /**
     * Thread Deconstructor
     */
    ~Thread();

    /**
     * Registers a function to be called when the thread initially is entered
     * and when the thread completes or is terminated.
     *
     * @param[in] notifyFunction The function that will be called.
     *  The first argument will contain a pointer to this thread.
     *  The second argument will be the threadID of this thread.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError registerEntryExitNotification(void(*notifyFunction)(Thread<T>, uint32_t));

    //TODO: do we really need a thread info get function?

    /**
     * Sets the preemptThreshold of the thread to the new values specified.
     *
     * @param[in] newThreshold The new value that the thread's preemptThreshold will be set to.
     * @param[out] oldThresholdOut A pointer to a space to store the previous value of the preemptThreshold
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError setPreemptThreshold(uint32_t newThreshold, uint32_t* oldThresholdOut);

    /**
     * Sets the priority of the thread to the new value specified.
     *
     * @param[in] newPriority The new value that the thread's priority will be set to.
     * @param[out] oldPriorityOut A pointer to a space to store the previous value of the priority.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError setPriority(uint32_t newPriority, uint32_t* oldPriorityOut);

    /**
     * Sets the timeSlice of the thread to the new value specified.
     *
     * @param[in] newTimeSlice The new value that the thread's timeSlice will be set to.
     * @param[out] oldTimeSliceOutput A pointer to a space to store the previous value of the timeSlice.
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError setTimeSlice(uint32_t newTimeSlice, uint32_t* oldTimeSliceOut);

    /**
     * Resets the thread so it is prepared to run the entry function again.
     *
     * @return Enum representing the first error found by the function (or Success if there was no error).
     */
    TXError reset();

    /**
     * Resumes the thread if it is suspended.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError resume();

    /**
     * Suspends the thread.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError suspend();

    /**
     * Terminates the thread. Once this thread is terminated, it cannot be restarted.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError terminate();

    /**
     * Aborts whatever wait this thread is in, returning WaitAborted to the method that this thread was waiting in.
     *
     * @return The first error found by the function (or Success if there was no error).
     */
    TXError abortWait();

private:
    /**
     * Threadx struct that holds the information for the thread.
     */
    TX_THREAD txThread;
    /**
     * Pointer to the name of this thread.
     */
    const char* name;
    /**
     * The function this thread will be running.
     */
    void (*entryFunction)(T*);
    /**
     * The data the thread function requires.
     */
    T* data;
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
     * Stores a notify function after the registerNotifyFunction method is called.
     * This is necessary so that programmers are given a thread object when their notify function is called
     * instead of TX's thread struct.
     */
    void(*storedNotifyFunction)(Thread<T>, uint32_t);

    /**
     * Method that will actually be registered with the tx kernel, just calls the stored notify function.
     */
    void memberNotifyFunction(TX_THREAD *thread, uint32_t threadID) {
        storedNotifyFunction(this, threadID);
    }

    /**
     * The type of notify function that threadx expects.
     */
    typedef void txNotifyFunction_t( TX_THREAD* , UINT );

    /**
     * A pointer to the function that we will register with the threadx kernel when the
     * registerNotificationFunction method is called. This function calls memberNotifyFunction, which itself calls
     * storedNotifyFunction, which will be set to the passed-in function for the registerNotifyFunction method.
     */
    txNotifyFunction_t *txNotifyFunction;
};

} // namespace core::rtos

#endif//EVT_RTOS_THREAD_
