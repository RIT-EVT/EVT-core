#include <core/dev/platform/f3xx/Timerf3xx.hpp>

#include <core/platform/f3xx/stm32f3xx.hpp>

TIM_HandleTypeDef halTimers[4];
void (*timerInterruptHandlers[4])(void* htim) = {nullptr};

enum class timerInterruptIndex {
    TIM2_IDX  = 0u,
    TIM15_IDX = 1u,
    TIM16_IDX = 2u,
    TIM17_IDX = 3u
};

uint8_t getTimerInterruptIndex(TIM_TypeDef* peripheral);

extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim) {
    TIM_TypeDef* peripheral = htim->Instance;
    IRQn_Type irqNum;

    if (peripheral == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        irqNum = TIM2_IRQn;
    } else if (peripheral == TIM15) {
        __HAL_RCC_TIM15_CLK_ENABLE();
        irqNum = TIM15_IRQn;
    } else if (peripheral == TIM16) {
        __HAL_RCC_TIM16_CLK_ENABLE();
        irqNum = TIM16_IRQn;
    } else if (peripheral == TIM17) {
        __HAL_RCC_TIM17_CLK_ENABLE();
        irqNum = TIM17_IRQn;
    } else {
        return; // Should never reach, but if an invalid peripheral is passed in then simply return
    }

    HAL_NVIC_SetPriority(irqNum, core::platform::TIMER_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(irqNum);
}

extern "C" void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim) {
    TIM_TypeDef* peripheral = htim->Instance;
    IRQn_Type irqNum;

    if (peripheral == TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
        irqNum = TIM2_IRQn;
    } else if (peripheral == TIM15) {
        __HAL_RCC_TIM15_CLK_DISABLE();
        irqNum = TIM15_IRQn;
    } else if (peripheral == TIM16) {
        __HAL_RCC_TIM16_CLK_DISABLE();
        irqNum = TIM16_IRQn;
    } else if (peripheral == TIM17) {
        __HAL_RCC_TIM17_CLK_DISABLE();
        irqNum = TIM17_IRQn;
    } else {
        return; // Should never reach, but if an invalid peripheral is passed in then simply return
    }

    HAL_NVIC_DisableIRQ(irqNum);
}

extern "C" void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM2)]);
}

extern "C" void TIM15_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM15)]);
}

extern "C" void TIM16_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM16)]);
}

extern "C" void TIM17_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM17)]);
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    uint8_t interruptIdx = getTimerInterruptIndex(htim->Instance);
    if (timerInterruptHandlers[interruptIdx] != nullptr) {
        timerInterruptHandlers[interruptIdx](htim);
    }
}

/**
 * Returns the corresponding index for the given timer device as stored in the
 * timerInterruptHandlers array
 * @param htim the TIM_TypeDef pointer to the timer device
 * @return an uint8_t between 0-3 corresponding to an element of timerInterruptHandlers
 */
uint8_t getTimerInterruptIndex(TIM_TypeDef* peripheral) {
    uint8_t interruptIdx;

    if (peripheral == TIM2) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM2_IDX);
    } else if (peripheral == TIM15) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM15_IDX);
    } else if (peripheral == TIM16) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM16_IDX);
    } else if (peripheral == TIM17) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM17_IDX);
    } else {
        interruptIdx = -1;
    }

    return interruptIdx;
}

namespace core::dev {

Timerf3xx::Timerf3xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod) {
    initTimer(timerPeripheral, clockPeriod);
    this->halTimer = &halTimers[getTimerInterruptIndex(timerPeripheral)];
}

void Timerf3xx::initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod) {
    this->clockPeriod = clockPeriod;
    auto& htim        = halTimers[getTimerInterruptIndex(timerPeripheral)];

    htim.Instance       = timerPeripheral;
    uint32_t prescaler  = HAL_RCC_GetHCLKFreq() / 1000;
    htim.Init.Prescaler = prescaler - 1; // Sets f_CK_PSC to 1000 Hz
    // Allows period increments of 1 ms with max of 2^(32) ms.
    htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim.Init.Period            = clockPeriod - 1;
    htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htim);

    TIM_ClockConfigTypeDef clockConfig   = {0};
    TIM_MasterConfigTypeDef masterConfig = {0};

    clockConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim, &clockConfig);

    masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    masterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);
}

void Timerf3xx::startTimer(void (*irqHandler)(void* htim)) {
    TIM_TypeDef* timerPeripheral = this->halTimer->Instance;
    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY) {
        stopTimer();
    }

    timerInterruptHandlers[getTimerInterruptIndex(timerPeripheral)] = irqHandler;
    startTimer();
}

void Timerf3xx::stopTimer() {
    HAL_TIM_Base_Stop_IT(this->halTimer);
}

void Timerf3xx::startTimer() {
    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY) {
        stopTimer(); // Stop timer in case it was already running
    }

    auto htim = this->halTimer;
    // Clear the interrupt flag so interrupt doesn't trigger immediately
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(htim);
}

void Timerf3xx::reloadTimer() {
    this->halTimer->Instance->CNT = 0; // Clear the Counter register to reset the timer
}

void Timerf3xx::setPeriod(uint32_t clockPeriod) {
    stopTimer();
    initTimer(this->halTimer->Instance, clockPeriod);
}
} // namespace core::dev
