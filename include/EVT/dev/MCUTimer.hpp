#ifndef _EVT_MCUTIMER_HPP_
#define _EVT_MCUTIMER_HPP_

#ifdef STM32F3xx
    #include <HALf3/stm32f3xx.h>
#endif
#ifdef STM32F4xx
    #include <HALf4/stm32f4xx.h>
#endif

namespace EVT::core::DEV {

/**
 * Enum for all the hardware timers available on the current MCU
 */
enum class MCUTimer {
#if defined(STM32F302x8)
    Timer1,
    Timer2,
    Timer6,
    Timer15,
    Timer16,
    Timer17,
#elif defined(STM32F334x8)
    Timer1,
    Timer2,
    Timer3,
    Timer6,
    Timer7,
    Timer15,
    Timer16,
    Timer17,
#elif defined(STM32F446xx)
    Timer1,
    Timer2,
    Timer3,
    Timer4,
    Timer5,
    Timer6,
    Timer7,
    Timer8,
    Timer9,
    Timer10,
    Timer11,
    Timer12,
    Timer13,
    Timer14,
#endif
};

/**
 * Gets the corresponding HAL TIM_TypeDef* for each MCUTimer
 *
 * @param mcuTimer MCUTimer of which to get the HAL equivalent
 * @return HAL equivalent of mcuTimer
 */
TIM_TypeDef* getTIM(MCUTimer mcuTimer) {
    TIM_TypeDef* timPeriph;
    switch (mcuTimer) {
#if defined(STM32F302x8)
    case MCUTimer::Timer1:
        timPeriph = TIM1;
    case MCUTimer::Timer2:
        timPeriph = TIM2;
    case MCUTimer::Timer6:
        timPeriph = TIM6;
    case MCUTimer::Timer15:
        timPeriph = TIM15;
    case MCUTimer::Timer16:
        timPeriph = TIM16;
    case MCUTimer::Timer17:
        timPeriph = TIM17;
#elif defined(STM32F334x8)
    case MCUTimer::Timer1:
        timPeriph = TIM1;
    case MCUTimer::Timer2:
        timPeriph = TIM2;
    case MCUTimer::Timer3:
        timPeriph = TIM3;
    case MCUTimer::Timer6:
        timPeriph = TIM6;
    case MCUTimer::Timer7:
        timPeriph = TIM7;
    case MCUTimer::Timer15:
        timPeriph = TIM15;
    case MCUTimer::Timer16:
        timPeriph = TIM16;
    case MCUTimer::Timer17:
        timPeriph = TIM17;
#elif defined(STM32F446xx)
    case MCUTimer::Timer1:
        timPeriph = TIM1;
    case MCUTimer::Timer2:
        timPeriph = TIM2;
    case MCUTimer::Timer3:
        timPeriph = TIM3;
    case MCUTimer::Timer4:
        timPeriph = TIM4;
    case MCUTimer::Timer5:
        timPeriph = TIM5;
    case MCUTimer::Timer6:
        timPeriph = TIM6;
    case MCUTimer::Timer7:
        timPeriph = TIM7;
    case MCUTimer::Timer8:
        timPeriph = TIM8;
    case MCUTimer::Timer9:
        timPeriph = TIM9;
    case MCUTimer::Timer10:
        timPeriph = TIM10;
    case MCUTimer::Timer11:
        timPeriph = TIM11;
    case MCUTimer::Timer12:
        timPeriph = TIM12;
    case MCUTimer::Timer13:
        timPeriph = TIM13;
    case MCUTimer::Timer14:
        timPeriph = TIM14;
#endif
    }

    return timPeriph;
}

}// namespace EVT::core::DEV

#endif//_EVT_MCUTIMER_HPP_
