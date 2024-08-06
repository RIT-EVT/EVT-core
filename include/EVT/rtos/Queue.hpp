#ifndef EVT_RTOS_QUEUE_
#define EVT_RTOS_QUEUE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

template <typename T>
class Queue : Initializable {
public:
    bool init(BytePool &pool) override;
private:

};

} // namespace core::rtos

#endif//EVT_RTOS_QUEUE_
