#include <EVT/dev/platform/f3xx/IWDGf3xx.hpp>

namespace EVT::core::DEV {

// According to a time formula in the documentation with the set prescaler,
// the ratio of counter ticks to milliseconds should be about 1:8. In testing,
// we found this to have an error of about 1 second
IWDGf3xx::IWDGf3xx(uint32_t ms) : halIWDG{
    IWDG1,
    {
        ms / 8 - 1,
        ms / 8 - 1,
        IWDG_PRESCALER_256,
    },
} {
    HAL_IWDG_Init(&halIWDG);
}

void IWDGf3xx::refresh() {
    HAL_IWDG_Refresh(&halIWDG);
}

}// namespace EVT::core::DEV
