#include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>
#include <HALf3/stm32f3xx_hal_conf.h>

extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    // TODO
}

namespace EVT::core::DEV {

Timerf302x8::Timerf302x8() {

}

void Timerf302x8::startTimer(uint32_t timerPeriod, void (*irqHandler)()) {

}

void Timerf302x8::stopTimer() {

}
}  // namespace EVT::core::DEV
