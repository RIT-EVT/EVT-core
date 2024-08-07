
#ifndef EVT_RTOS_THREADX_
#define EVT_RTOS_THREADX_

#include <EVT/rtos/Initializable.hpp>
#include <cstdint>

namespace core::rtos {

/**
 * Allocates a bytepool to store all of the Initializable objects passed into
 * this function, initializes them, and begins the threadx kernel.
 *
 * @param initList the list of Initializeable objects that must be initialized
 * before the threadx kernel begins.
 * @param length the length of initList.
 * @param poolptr a reference to the bytepool to store the information of the initializable objects in.
 */
void init(Initializable* initList, std::size_t length, BytePool &poolptr);

} // namespace core::rtos

#endif//EVT_RTOS_THREADX_
