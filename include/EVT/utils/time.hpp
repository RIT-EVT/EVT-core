/**
 * The functions defined in here are used for time based operations. All
 * operations are platform independent.
 *
 */

#ifndef _EVT_TIME_
#define _EVT_TIME_

#include <cstdint>

#include <HALf3/stm32f3xx_hal.h>

namespace EVT::core::time
{
    /**
     * Function to have the program hold for a set amount of time before
     * continuing.
     *
     * @param ms The number of milliseconds to wait for
     */
    void wait(uint32_t ms);
}
#endif
