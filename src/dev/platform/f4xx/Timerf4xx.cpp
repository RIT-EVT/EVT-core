#include <EVT/dev/platform/f4xx/Timerf4xx.hpp>

#include <EVT/platform/f4xx/stm32f4xx.hpp>

TIM_HandleTypeDef halTimers[10];
void (*timerInterruptHandlers[10])(void* htim) = {nullptr};

enum class timerInterruptIndex {
    TIM2_IDX = 0u,
    TIM3_IDX = 1u,
    TIM4_IDX = 2u,
    TIM5_IDX = 3u,
    TIM9_IDX = 4u,
    TIM10_IDX = 5u,
    TIM11_IDX = 6u,
    TIM12_IDX = 7u,
    TIM13_IDX = 8u,
    TIM14_IDX = 9u
};

uint8_t getTimerInterruptIndex(TIM_TypeDef* peripheral);

extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim) {
    TIM_TypeDef* peripheral = htim->Instance;
    IRQn_Type irqNum;

    if (peripheral == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        irqNum = TIM2_IRQn;
    } else if (peripheral == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
        irqNum = TIM3_IRQn;
    } else if (peripheral == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE();
        irqNum = TIM4_IRQn;
    } else if (peripheral == TIM5) {
        __HAL_RCC_TIM5_CLK_ENABLE();
        irqNum = TIM5_IRQn;
    } else if (peripheral == TIM9) {
        __HAL_RCC_TIM9_CLK_ENABLE();
        irqNum = TIM1_BRK_TIM9_IRQn;
    } else if (peripheral == TIM10) {
        __HAL_RCC_TIM10_CLK_ENABLE();
        irqNum = TIM1_UP_TIM10_IRQn;
    } else if (peripheral == TIM11) {
        __HAL_RCC_TIM11_CLK_ENABLE();
        irqNum = TIM1_TRG_COM_TIM11_IRQn;
    } else if (peripheral == TIM12) {
        __HAL_RCC_TIM12_CLK_ENABLE();
        irqNum = TIM8_BRK_TIM12_IRQn;
    } else if (peripheral == TIM13) {
        __HAL_RCC_TIM13_CLK_ENABLE();
        irqNum = TIM8_UP_TIM13_IRQn;
    } else if (peripheral == TIM14) {
        __HAL_RCC_TIM14_CLK_ENABLE();
        irqNum = TIM8_TRG_COM_TIM14_IRQn;
    } else {
        return;// Should never reach, but if an invalid peripheral is passed in then simply return
    }

    HAL_NVIC_SetPriority(irqNum, EVT::core::platform::TIMER_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(irqNum);
}

extern "C" void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim) {
    TIM_TypeDef* peripheral = htim->Instance;
    IRQn_Type irqNum;

    if (peripheral == TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
        irqNum = TIM2_IRQn;
    } else if (peripheral == TIM3) {
        __HAL_RCC_TIM3_CLK_DISABLE();
        irqNum = TIM3_IRQn;
    } else if (peripheral == TIM4) {
        __HAL_RCC_TIM4_CLK_DISABLE();
        irqNum = TIM4_IRQn;
    } else if (peripheral == TIM5) {
        __HAL_RCC_TIM5_CLK_DISABLE();
        irqNum = TIM5_IRQn;
    } else if (peripheral == TIM9) {
        __HAL_RCC_TIM9_CLK_DISABLE();
        irqNum = TIM1_BRK_TIM9_IRQn;
    } else if (peripheral == TIM10) {
        __HAL_RCC_TIM10_CLK_DISABLE();
        irqNum = TIM1_UP_TIM10_IRQn;
    } else if (peripheral == TIM11) {
        __HAL_RCC_TIM11_CLK_DISABLE();
        irqNum = TIM1_TRG_COM_TIM11_IRQn;
    } else if (peripheral == TIM12) {
        __HAL_RCC_TIM12_CLK_DISABLE();
        irqNum = TIM8_BRK_TIM12_IRQn;
    } else if (peripheral == TIM13) {
        __HAL_RCC_TIM13_CLK_DISABLE();
        irqNum = TIM8_UP_TIM13_IRQn;
    } else if (peripheral == TIM14) {
        __HAL_RCC_TIM14_CLK_DISABLE();
        irqNum = TIM8_TRG_COM_TIM14_IRQn;
    } else {
        return;// Should never reach, but if an invalid peripheral is passed in then simply return
    }

    HAL_NVIC_DisableIRQ(irqNum);
}

extern "C" void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM2)]);
}

extern "C" void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM3)]);
}

extern "C" void TIM4_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM4)]);
}

extern "C" void TIM5_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM5)]);
}

extern "C" void TIM1_BRK_TIM9_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM9)]);
}

extern "C" void TIM1_UP_TIM10_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM10)]);
}

extern "C" void TIM1_TRG_COM_TIM11_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM11)]);
}

extern "C" void TIM8_BRK_TIM12_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM12)]);
}

extern "C" void TIM8_UP_TIM13_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM13)]);
}

extern "C" void TIM8_TRG_COM_TIM14_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM14)]);
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
 * @return an uint8_t between 0-9 corresponding to an element of timerInterruptHandlers
 */
uint8_t getTimerInterruptIndex(TIM_TypeDef* peripheral) {
    uint8_t interruptIdx;

    if (peripheral == TIM2) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM2_IDX);
    } else if (peripheral == TIM3) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM3_IDX);
    } else if (peripheral == TIM4) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM4_IDX);
    } else if (peripheral == TIM5) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM5_IDX);
    } else if (peripheral == TIM9) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM9_IDX);
    } else if (peripheral == TIM10) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM10_IDX);
    } else if (peripheral == TIM11) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM11_IDX);
    } else if (peripheral == TIM12) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM12_IDX);
    } else if (peripheral == TIM13) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM13_IDX);
    } else if (peripheral == TIM14) {
        interruptIdx = static_cast<uint8_t>(timerInterruptIndex::TIM14_IDX);
    } else {
        interruptIdx = -1;
    }

    return interruptIdx;
}

namespace EVT::core::DEV {

Timerf4xx::Timerf4xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod) {
    initTimer(timerPeripheral, clockPeriod);
    this->halTimer = &halTimers[getTimerInterruptIndex(timerPeripheral)];
}

void Timerf4xx::initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod) {
    this->clockPeriod = clockPeriod;
    auto& htim = halTimers[getTimerInterruptIndex(timerPeripheral)];

    htim.Instance = timerPeripheral;
    uint32_t prescaler = SystemCoreClock / 1000;
    htim.Init.Prescaler = prescaler;// Sets f_CK_PSC to 1000 Hz
    // Allows period increments of 1 ms with max of 2^(32) ms.
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = clockPeriod;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htim);

    TIM_ClockConfigTypeDef clockConfig = {0};
    TIM_MasterConfigTypeDef masterConfig = {0};

    clockConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim, &clockConfig);

    // Timers 9-14 are NOT master mode compatible, so waste of time to go through config
    if (getTimerInterruptIndex(timerPeripheral) < static_cast<uint8_t>(timerInterruptIndex::TIM9_IDX)) {
        masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);
    }
}

void Timerf4xx::startTimer(void (*irqHandler)(void* htim)) {
    TIM_TypeDef* timerPeripheral = this->halTimer->Instance;
    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY)
        stopTimer();

    timerInterruptHandlers[getTimerInterruptIndex(timerPeripheral)] = irqHandler;
    startTimer();
}

void Timerf4xx::stopTimer() {
    HAL_TIM_Base_Stop_IT(this->halTimer);
}

void Timerf4xx::startTimer() {
    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY)
        stopTimer();// Stop timer in case it was already running

    auto htim = this->halTimer;
    // Clear the interrupt flag so interrupt doesn't trigger immediately
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(htim);
}

void Timerf4xx::reloadTimer() {
    this->halTimer->Instance->CNT &= ~(0xFFFFFFFF);// Clear the Counter register to reset the timer
}

void Timerf4xx::setPeriod(uint32_t clockPeriod) {
    stopTimer();
    initTimer(this->halTimer->Instance, clockPeriod);
}
}// namespace EVT::core::DEV
