/**
 * STMF3xx implementation of the time functionality. This code is designed
 * only to run on the STMF3xx.
 */

#include <HALf4/stm32f4xx_hal.h>
#include <HALf4/stm32f4xx_it.h>

#include <core/utils/time.hpp>
#include <cstdint>

namespace core::time {
/**
 * Calls HAL wait, assumes HAL_Init() has been called
 */
void wait(uint32_t ms) { HAL_Delay(ms); }

/**
 * Reads the system ticks from HAL_GetTicks
 */
uint32_t millis() { return HAL_GetTick(); }
} // namespace core::time
