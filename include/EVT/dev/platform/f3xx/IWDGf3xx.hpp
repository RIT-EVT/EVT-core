#ifndef _EVT_IWDGf3xx_
#define _EVT_IWDGf3xx_

#include <HALf3/stm32f3xx.h>

#include <EVT/dev/IWDG.hpp>

namespace EVT::core::DEV {

/**
 * Implementation of the independent watchdog for STM32f3xx MCUs, which
 * use the low-speed internal RC oscillator to detect software failures and
 * trigger a reset.
 */
class IWDGf3xx : public IWDG {
public:
    /**
     * Constructor for the IWDG, which also starts the watchdog timer.
     * @param ms Time in milliseconds before the IWDG triggers a reset,
     * must be a value between 8 and 32768 ms.
     */
    explicit IWDGf3xx(uint32_t ms);

    void refresh();

private:
    /** Instance of the HAL IWDG */
    IWDG_HandleTypeDef halIWDG;
};

}// namespace EVT::core::DEV

#endif
