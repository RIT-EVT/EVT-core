#ifndef _EVT_TIMER3xx_H
#define _EVT_TIMER3xx_H

#include <HALf3/stm32f3xx.h>

#include <core/dev/platform/f3xx/Timerf3xx.hpp>

namespace core::dev {

class PWMTimerF3xx : public TimerF3xx {
public:
    explicit PWMTimerF3xx();
};

} // namespace core::dev

#endif //_EVT_TIMER3xx_H
