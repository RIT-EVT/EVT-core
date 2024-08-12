
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
     * @param entryFunction Function the thread will be running.
     * @param data Data the thread's function requires.
     * @param stackSize How much stack space this thread is allocated.
     */
    Thread(void (*entryFunction)(T), T data, std::size_t stackSize);

    /**
     * Creates the threadx thread and starts it.
     * @return whether or not the thread was started successfully.
     */
    TXError init(BytePoolBase &pool) override;
private:
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
};

} // namespace core::rtos

#endif//EVT_RTOS_THREAD_
