#ifndef EVT_RTOS_SEMAPHORE_
#define EVT_RTOS_SEMAPHORE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

template <typename T>
class Semaphore : Initializable {
public:
    bool init(BytePool &pool) override;
private:

};

} // namespace core::rtos

#endif//EVT_RTOS_SEMAPHORE_
