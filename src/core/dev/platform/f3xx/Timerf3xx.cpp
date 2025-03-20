#include <core/dev/platform/f3xx/Timerf3xx.hpp>

#include <core/platform/f3xx/stm32f3xx.hpp>

#if defined(STM32F302x8)
#define F302_TIMER_COUNT 5

TIM_HandleTypeDef halTimers[F302_TIMER_COUNT];
void (*timerInterruptHandlers[F302_TIMER_COUNT])(void* htim) = {nullptr};

// Timer 6 is technically available but does not support Capture Compare channels, so is therefore not supported.
enum timerInterruptIndex {
    TIM1_IDX  = 0u,
    TIM2_IDX  = 1u,
    TIM15_IDX = 2u,
    TIM16_IDX = 3u,
    TIM17_IDX = 4u,
    NO_IDX = -1
};

/**
 * Returns the corresponding index for the given timer device as stored in the
 * timerInterruptHandlers array
 * @param htim the TIM_TypeDef pointer to the timer device
 * @return an uint8_t between 0-3 corresponding to an element of timerInterruptHandlers
 */
timerInterruptIndex getTimerInterruptIndex(TIM_TypeDef* peripheral) {
    timerInterruptIndex interruptIdx;

    if (peripheral == TIM1) {
        interruptIdx = TIM1_IDX;
    } else if (peripheral == TIM2) {
        interruptIdx = TIM2_IDX;
    } else if (peripheral == TIM15) {
        interruptIdx = TIM15_IDX;
    } else if (peripheral == TIM16) {
        interruptIdx = TIM16_IDX;
    } else if (peripheral == TIM17) {
        interruptIdx = TIM17_IDX;
    } else {
        interruptIdx = NO_IDX;
    }

    return interruptIdx;
}
extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim) {
    TIM_TypeDef* peripheral = htim->Instance;
    IRQn_Type irqNum;

    if (peripheral == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
        irqNum = TIM1_CC_IRQn;
    } else if (peripheral == TIM2) {
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

    if (peripheral == TIM1) {
      __HAL_RCC_TIM1_CLK_DISABLE();
        irqNum = TIM1_CC_IRQn;
    } else if (peripheral == TIM2) {
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
#elif defined(STM32F334x8)
#define F334_TIMER_COUNT 6

TIM_HandleTypeDef halTimers[F334_TIMER_COUNT];
void (*timerInterruptHandlers[F334_TIMER_COUNT])(void* htim) = {nullptr};

// Timer 6 is technically available but does not support Capture Compare channels, so is therefore not supported.
enum timerInterruptIndex {
    TIM1_IDX  = 0u,
    TIM2_IDX  = 1u,
    TIM3_IDX  = 2u,
    TIM15_IDX = 3u,
    TIM16_IDX = 4u,
    TIM17_IDX = 5u,
    NO_IDX = -1
};

/**
 * Returns the corresponding index for the given timer device as stored in the
 * timerInterruptHandlers array
 * @param htim the TIM_TypeDef pointer to the timer device
 * @return an uint8_t between 0-3 corresponding to an element of timerInterruptHandlers
 */
timerInterruptIndex getTimerInterruptIndex(TIM_TypeDef* peripheral) {
    timerInterruptIndex interruptIdx;

    if (peripheral == TIM1) {
        interruptIdx = TIM1_IDX;
    } else if (peripheral == TIM2) {
        interruptIdx = TIM2_IDX;
    } else if (peripheral == TIM3) {
        interruptIdx = TIM3_IDX;
    } else if (peripheral == TIM15) {
        interruptIdx = TIM15_IDX;
    } else if (peripheral == TIM16) {
        interruptIdx = TIM16_IDX;
    } else if (peripheral == TIM17) {
        interruptIdx = TIM17_IDX;
    } else {
        interruptIdx = NO_IDX;
    }

    return interruptIdx;
}
extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim) {
    TIM_TypeDef* peripheral = htim->Instance;
    IRQn_Type irqNum;

    if (peripheral == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
        irqNum = TIM1_CC_IRQn;
    } else if (peripheral == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        irqNum = TIM2_IRQn;
    } else if (peripheral == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
        irqNum = TIM3_IRQn;
    }  else if (peripheral == TIM15) {
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

    if (peripheral == TIM1) {
      __HAL_RCC_TIM1_CLK_DISABLE();
        irqNum = TIM1_CC_IRQn;
    } else if (peripheral == TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
        irqNum = TIM2_IRQn;
    } else if (peripheral == TIM3) {
        __HAL_RCC_TIM3_CLK_DISABLE();
        irqNum = TIM3_IRQn;
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

extern "C" void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM3)]);
}
#endif

// Common IRQHandlers between both f3s
extern "C" void TIM1_IRQHandler(void) {
    HAL_TIM_IRQHandler(&halTimers[getTimerInterruptIndex(TIM1)]);
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

namespace core::dev {

TimerF3xx::TimerF3xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, TimerConfiguration configuration, uint32_t clockPrescaler) : configuration(configuration) {
    this->halTimer = &halTimers[getTimerInterruptIndex(timerPeripheral)];
    initTimer(timerPeripheral, clockPeriod, clockPrescaler);
}

void TimerF3xx::initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, uint32_t clockPrescaler) {
    this->clockPeriod = clockPeriod;
    auto& htim        = halTimers[getTimerInterruptIndex(timerPeripheral)];

    htim.Instance       = timerPeripheral;
    if (clockPrescaler == AUTO_PRESCALER) {
        uint32_t prescaler  = HAL_RCC_GetHCLKFreq() / 1000;
        htim.Init.Prescaler = prescaler - 1; // Sets f_CK_PSC to 1000 Hz
    } else {
        htim.Init.Prescaler = clockPrescaler;
    }
    // Allows period increments of 1 ms with max of 2^(32) ms.
    htim.Init.CounterMode       = this->configuration.counterMode;
    htim.Init.Period            = clockPeriod - 1;
    htim.Init.ClockDivision     = this->configuration.clockDivision;
    htim.Init.AutoReloadPreload = this->configuration.autoReloadPreload;
    HAL_TIM_Base_Init(&htim);

    TIM_ClockConfigTypeDef clockConfig   = {0};
    TIM_MasterConfigTypeDef masterConfig = {0};

    clockConfig.ClockSource = this->configuration.clockSource;
    HAL_TIM_ConfigClockSource(&htim, &clockConfig);

    masterConfig.MasterOutputTrigger = this->configuration.masterOutputTrigger;
    masterConfig.MasterSlaveMode     = this->configuration.masterSlaveMode;
    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);
}

void TimerF3xx::startTimer(void (*irqHandler)(void* htim)) {
    TIM_TypeDef* timerPeripheral = this->halTimer->Instance;
    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY) {
        stopTimer();
    }

    timerInterruptHandlers[getTimerInterruptIndex(timerPeripheral)] = irqHandler;
    startTimer();
}

void TimerF3xx::stopTimer() {
    HAL_TIM_Base_Stop_IT(this->halTimer);
}

void TimerF3xx::startTimer() {
    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY) {
        stopTimer(); // Stop timer in case it was already running
    }

    auto htim = this->halTimer;
    // Clear the interrupt flag so interrupt doesn't trigger immediately
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(htim);
}

void TimerF3xx::reloadTimer() {
    this->halTimer->Instance->CNT = 0; // Clear the Counter register to reset the timer
}

void TimerF3xx::setPeriod(uint32_t clockPeriod, uint32_t clockPrescaler) {
    stopTimer();
    initTimer(this->halTimer->Instance, clockPeriod, clockPrescaler);
}
} // namespace core::dev
