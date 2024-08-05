
#ifndef EVT_RTOS_THREAD_
#define EVT_RTOS_THREAD_

#include <EVT/rtos/Initializable.hpp>
#include <cstdint>

namespace core::rtos {

template <typename T>
class Thread : Initializable {
public:
    Thread(void (*entryFunction)(T), T data, std::size_t stackSize);
    bool init();
private:
    T data;
    void (*entryFunction)(T);
    std::size_t stackSize;
};

} // namespace core::rtos

#endif//EVT_RTOS_THREAD_
