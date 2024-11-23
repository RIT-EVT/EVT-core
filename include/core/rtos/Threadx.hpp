#ifndef EVT_RTOS_THREADX_
#define EVT_RTOS_THREADX_

#include <core/rtos/Initializable.hpp>
#include <cstdint>

#define S_TO_TICKS(n)  ((uint32_t) (n) *TX_TIMER_TICKS_PER_SECOND)
#define MS_TO_TICKS(n) ((uint32_t) (n) *TX_TIMER_TICKS_PER_SECOND / 1000u)

namespace core::rtos {

/**
 * Allocate a bytepool to store all of the Initializable objects passed into this function, initialize them, and begin
 * the threadx kernel
 *
 * @param[in] initList The list of Initializable objects that must be initialized before the threadx kernel begins
 * @param length The length of initList
 * @param[in] poolptr A reference to the bytepool to store the information of the initializable objects in
 * @return The first error found by the function or TXE_SUCCESS if there was no error
 */
TXError startKernel(Initializable** initList, std::size_t length, BytePoolBase& pool);

/**
 * Initialize all members of initList into the current BytePool
 *
 * @param[in] initList The list of Initializable objects that must be initialized
 * before the threadx kernel begins
 * @param length The length of initList
 * @param[in] poolptr A reference to the bytepool to store the information of the initializable objects in
 * @return The first error found by the function or TXE_SUCCESS if there was no error
 */
TXError bulkInitialize(Initializable** initList, std::size_t length, BytePoolBase& pool);

/**
 * Relinquish control of the cpu from the currently running thread, allowing the
 * threadx kernel to potentially pick another thread to run.
 * If the currently running thread is the highest priority thread running, this method will not have any effect
 */
void relinquish();

/**
 * Sleep the currently running thread for the given amount of time. If no thread is running returns an error
 *
 * @param sleepTime How long (in ticks) the thread should sleep for
 * @return The first error found by the function or TXE_SUCCESS if there was no error
 */
TXError sleep(uint32_t sleepTime);

} // namespace core::rtos

#endif // EVT_RTOS_THREADX_
