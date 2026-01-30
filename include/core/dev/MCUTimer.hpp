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
    None
};

/**
 * Gets the corresponding HAL TIM_TypeDef* for each MCUTimer
 * This function is inlined because of a couple of reasons.
 * 1. It is in a header file, and functions in header files should be inlined.
 * https://clang.llvm.org/extra/clang-tidy/checks/misc/definitions-in-headers.html
 * 2. When this function is not inlined, F3 targets will not build. This is because PWMF3xx requires its own
 *    `timerInstance` function that requires an MCUTimer import. Since manager.hpp also includes MCUTimer.hpp, and has
 *    generally broken imports (see [FWT-255] https://rit-evt.atlassian.net/browse/FWT-255).
 *
 * @param mcuTimer MCUTimer of which to get the HAL equivalent
 * @return HAL equivalent of mcuTimer
 */
inline TIM_TypeDef* getTIM(const MCUTimer mcuTimer) {
    TIM_TypeDef* timPeriph;
    switch (mcuTimer) {
#if defined(STM32F302x8)
    case MCUTimer::Timer1:
        timPeriph = TIM1;
        break;
    case MCUTimer::Timer2:
        timPeriph = TIM2;
        break;
    case MCUTimer::Timer15:
        timPeriph = TIM15;
        break;
    case MCUTimer::Timer16:
        timPeriph = TIM16;
        break;
    case MCUTimer::Timer17:
        timPeriph = TIM17;
        break;
    default:
        timPeriph = TIM1;
        break;
#elif defined(STM32F334x8)
    case MCUTimer::Timer1:
        timPeriph = TIM1;
        break;
    case MCUTimer::Timer2:
        timPeriph = TIM2;
        break;
    case MCUTimer::Timer3:
        timPeriph = TIM3;
        break;
    case MCUTimer::Timer15:
        timPeriph = TIM15;
        break;
    case MCUTimer::Timer16:
        timPeriph = TIM16;
        break;
    case MCUTimer::Timer17:
        timPeriph = TIM17;
        break;
    default:
        timPeriph = TIM1;
        break;
#elif defined(STM32F446xx)
    case MCUTimer::Timer1:
        timPeriph = TIM1;
        break;
    case MCUTimer::Timer2:
        timPeriph = TIM2;
        break;
    case MCUTimer::Timer3:
        timPeriph = TIM3;
        break;
    case MCUTimer::Timer4:
        timPeriph = TIM4;
        break;
    case MCUTimer::Timer5:
        timPeriph = TIM5;
        break;
    case MCUTimer::Timer8:
        timPeriph = TIM8;
        break;
    case MCUTimer::Timer9:
        timPeriph = TIM9;
        break;
    case MCUTimer::Timer10:
        timPeriph = TIM10;
        break;
    case MCUTimer::Timer11:
        timPeriph = TIM11;
        break;
    case MCUTimer::Timer12:
        timPeriph = TIM12;
        break;
    case MCUTimer::Timer13:
        timPeriph = TIM13;
        break;
    case MCUTimer::Timer14:
        timPeriph = TIM14;
        break;
    default:
        timPeriph = TIM1;
        break;
#endif
    }

    return timPeriph;
}

} // namespace core::dev

#endif //_EVT_MCUTIMER_HPP_
