#include <EVT/dev/platform/f4xx/IWDGf4xx.hpp>

namespace EVT::core::DEV {

// According to a time formula in the documentation with the set prescaler,
// the ratio of counter ticks to milliseconds should be about 1:8. In testing,
// we found this to have an error of about 1 second
IWDGf4xx::IWDGf4xx(uint32_t ms) : halIWDG{
    IWDG1,
    {
        IWDG_PRESCALER_256,
        ms / 8 - 1,
    },
} {}

void IWDGf4xx::init() {
    HAL_IWDG_Init(&halIWDG);
    isActive = true;
}

void IWDGf4xx::refresh() {
    if (isActive) {
        HAL_IWDG_Refresh(&halIWDG);
    }
}

}// namespace EVT::core::DEV
