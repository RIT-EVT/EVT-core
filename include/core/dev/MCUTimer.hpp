#ifndef _EVT_MCUTIMER_HPP_
#define _EVT_MCUTIMER_HPP_

#ifdef STM32F3xx
    #include <HALf3/stm32f3xx.h>
#endif
#ifdef STM32F4xx
    #include <HALf4/stm32f4xx.h>
#endif

namespace core::dev {
/**
 * Enum for all the hardware timers available on the current MCU
 */
enum class MCUTimer {
#if defined(STM32F302x8)
    Timer1,
    Timer2,
    Timer15,
    Timer16,
    Timer17,
#elif defined(STM32F334x8)
    Timer1,
    Timer2,
    Timer3,
    Timer15,
    Timer16,
    Timer17,
#elif defined(STM32F446xx)
    Timer1,
    Timer2,
    Timer3,
    Timer4,
    Timer5,
    Timer8,
    Timer9,
    Timer10,
    Timer11,
    Timer12,
    Timer13,
    Timer14,
#endif
    None,

};
} // namespace core::dev

#endif //_EVT_MCUTIMER_HPP_
