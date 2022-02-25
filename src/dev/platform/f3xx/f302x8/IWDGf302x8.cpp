#include <EVT/dev/platform/f3xx/f302x8/IWDGf302x8.hpp>

namespace EVT::core::DEV {

IWDGf302x8::IWDGf302x8(uint32_t ms) {
    // According to a time formula in the documentation with the set prescaler,
    // the ratio of counter ticks to milliseconds should be about 1:8. In testing,
    // we found this to have an error of about 1 second
    uint32_t windowSize = ms / 8;

    halIWDG.Instance = IWDG1;
    halIWDG.Init.Reload = windowSize - 1;
    halIWDG.Init.Window = windowSize - 1;
    halIWDG.Init.Prescaler = IWDG_PRESCALER_256;

    HAL_IWDG_Init(&halIWDG);
}

void IWDGf302x8::refresh() {
    HAL_IWDG_Refresh(&halIWDG);
}

}// namespace EVT::core::DEV