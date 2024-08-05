
#ifndef EVT_RTOS_THREADX_
#define EVT_RTOS_THREADX_

#include <EVT/rtos/Initializable.hpp>
#include <cstdint>

namespace core::rtos {

template <std::size_t poolSize>
void init(Initializable* initList, std::size_t length);

} // namespace core::rtos

#endif//EVT_RTOS_THREADX_
