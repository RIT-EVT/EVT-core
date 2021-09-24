#include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>
#include <HALf3/stm32f3xx_hal_conf.h>

TIM_HandleTypeDef halTimer;

extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    // TODO: Use htim to determine which device to enable

    // Enable TIM2 device
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Setup general interrupt for TIM2
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

extern "C" void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimer);
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // Handle interrupt here
}

namespace EVT::core::DEV {

Timerf302x8::Timerf302x8() {
    halTimer.Instance = TIM2;


    HAL_TIM_Base_MspInit(&halTimer);
}

void Timerf302x8::startTimer(uint32_t timerPeriod, void (*irqHandler)()) {

}

void Timerf302x8::stopTimer() {

}
}  // namespace EVT::core::DEV
