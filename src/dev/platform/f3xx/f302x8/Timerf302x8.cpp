#include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>
#include <HALf3/stm32f3xx_hal_conf.h>
#include <HALf3/stm32f3xx_it.h>
#include <EVT/platform/f3xx/stm32f302x8.hpp>

TIM_HandleTypeDef halTimer;
void (*timerInterruptHandler)(TIM_HandleTypeDef *htim) = nullptr;

extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    // TODO: Use htim to determine which device to enable

    // Enable TIM2 device
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Setup general interrupt for TIM2
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

extern "C" void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim) {
    __HAL_RCC_TIM2_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
}

extern "C" void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimer);
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (timerInterruptHandler != nullptr) {
        timerInterruptHandler(htim);
    }
}

namespace EVT::core::DEV {

Timerf302x8::Timerf302x8(uint32_t clockPeriod, void (*irqHandler)(TIM_HandleTypeDef *htim)) {
    timerInterruptHandler = irqHandler;
    initTimer(clockPeriod);
}

void Timerf302x8::initTimer(uint32_t clockPeriod) {
    this->clockPeriod = clockPeriod;

    halTimer.Instance = TIM2;
    uint32_t prescaler = EVT::core::platform::CLK_SPEED / 1000;
    halTimer.Init.Prescaler = prescaler;    // Sets f_CK_PSC to 1000 Hz
    // Allows period increments of 1 ms with max of 2^(32) ms.
    halTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
    halTimer.Init.Period = clockPeriod;
    halTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    halTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&halTimer);

    TIM_ClockConfigTypeDef clockConfig = {0};
    TIM_MasterConfigTypeDef masterConfig = {0};

    clockConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&halTimer, &clockConfig);

    masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&halTimer, &masterConfig);

    HAL_TIM_Base_Start_IT(&halTimer);
}


void Timerf302x8::startTimer(void (*irqHandler)(TIM_HandleTypeDef *htim)) {
    stopTimer();
    timerInterruptHandler = irqHandler;
    initTimer(this->clockPeriod);
}

void Timerf302x8::stopTimer() {
    HAL_TIM_Base_MspDeInit(&halTimer);
    timerInterruptHandler = nullptr;
}

void Timerf302x8::startTimer() {
    stopTimer();  // Stop timer in case it was already running
    initTimer(this->clockPeriod);
}
}  // namespace EVT::core::DEV
