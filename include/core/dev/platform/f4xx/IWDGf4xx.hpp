#ifndef EVT_IWDGF4XX_HPP
#define EVT_IWDGF4XX_HPP

#include <HALf4/stm32f4xx.h>

#include <core/dev/IWDG.hpp>

namespace core::dev {

/**
 * Implementation of the independent watchdog for STM32f3xx MCUs, which
 * use the low-speed internal RC oscillator to detect software failures and
 * trigger a reset.
 */
class IWDGf4xx : public IWDG {
public:
    /**
     * Constructor for the IWDG, which also starts the watchdog timer.
     *
     * @param ms Time in milliseconds before the IWDG triggers a reset,
     * must be a value between 8 and 32768 ms.
     */
    explicit IWDGf4xx(uint32_t ms);

    void init() override;

    void refresh() override;

private:
    /** Instance of the HAL IWDG */
    IWDG_HandleTypeDef halIWDG;

    /** Whether the watchdog timer is active */
    bool isActive = false;
};

} // namespace core::dev

#endif // EVT_IWDGF4XX_HPP
