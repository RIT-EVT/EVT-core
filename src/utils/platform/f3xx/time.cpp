/**
 * STMF3xx implementation of the time functionality. This code is designed
 * only to run on the STMF3xx.
 */

#include <EVT/utils/time.hpp>

#include <HALf3/stm32f3xx_hal.h>
#include <HALf3/stm32f3xx_it.h>

namespace EVT::core::time
{
    /**
     * Calls HAL wait, assumes HAL_Init() has been called
     */
    void wait(uint32_t ms)
    {
        // TODO: Remove need for this first time call
        static bool isFirst = true;
        if(isFirst)
        {
            HAL_Init();
            SysTick_Handler();
        }

        HAL_Delay(ms);
    }
}
