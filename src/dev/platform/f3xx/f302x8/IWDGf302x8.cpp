#include <EVT/dev/platform/f3xx/f302x8/IWDGf302x8.hpp>

namespace EVT::core::DEV {

IWDGf302x8::IWDGf302x8(uint32_t windowSize) {
    halIWDG.Instance = IWDG1;
    halIWDG.Init.Window = windowSize;

    // Numbers generated from STMCubeMX
    halIWDG.Init.Reload = 4095;
    halIWDG.Init.Prescaler = IWDG_PRESCALER_4;

    HAL_IWDG_Init(&halIWDG);
}

void IWDGf302x8::refresh() {
    HAL_IWDG_Refresh(&halIWDG);
}

} // namespace EVT::core::DEV