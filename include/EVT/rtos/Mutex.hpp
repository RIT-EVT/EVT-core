#ifndef EVT_RTOS_MUTEX_
#define EVT_RTOS_MUTEX_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Mutex : Initializable {
public:
    bool init(BytePool &pool) override;
private:
};

} // namespace core::rtos

#endif//EVT_RTOS_MUTEX_
