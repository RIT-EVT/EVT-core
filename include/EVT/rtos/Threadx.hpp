
#ifndef EVT_RTOS_THREADX_
#define EVT_RTOS_THREADX_

#include <EVT/rtos/Initializable.hpp>
#include <cstdint>

namespace core::rtos {

/**
 * Allocates a bytepool to store all of the Initializable objects passed into
 * this function, initializes them, and begins the threadx kernel.
 *
 * @tparam poolSize how big the bytepool should be.
 * @param initList the list of Initializeable objects that must be initialized
 * before the threadx kernel begins.
 * @param length the length of initList.
 * @param pool the bytepool to store the initializable objects in.
 */
template <std::size_t poolSize>
void init(Initializable* initList, std::size_t length, BytePool &pool);

} // namespace core::rtos

#endif//EVT_RTOS_THREADX_
