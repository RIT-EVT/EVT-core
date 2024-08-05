#ifndef EVT_RTOS_MUTEX_
#define EVT_RTOS_MUTEX_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

template <typename T>
class Mutex : Initializable {
public:
    bool init(BytePool &pool);
private:
};

} // namespace core::rtos

#endif//EVT_RTOS_MUTEX_
