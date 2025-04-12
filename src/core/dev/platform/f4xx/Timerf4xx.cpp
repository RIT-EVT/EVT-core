#include <core/dev/platform/f4xx/Timerf4xx.hpp>

#include <core/platform/f4xx/stm32f4xx.hpp>
#include <core/utils/log.hpp>
#define F4_TIMER_COUNT 12

TIM_HandleTypeDef halTimers[F4_TIMER_COUNT];
void (*timerInterruptHandlers[F4_TIMER_COUNT])(void* htim) = {nullptr};

enum timerInterruptIndex {
    TIM1_IDX  = 0u,
    TIM2_IDX  = 1u,
    TIM3_IDX  = 2u,
    TIM4_IDX  = 3u,
    TIM5_IDX  = 4u,
    TIM8_IDX  = 5u,
    TIM9_IDX  = 6u,
    TIM10_IDX = 7u,
    TIM11_IDX = 8u,
    TIM12_IDX = 9u,
    TIM13_IDX = 10u,
    TIM14_IDX = 11u,
    NO_IDX = -1
};

/**
 * Returns the corresponding index for the given timer device as stored in the
 * timerInterruptHandlers array
 * @param htim the TIM_TypeDef pointer to the timer device
 * @return an uint8_t between 0-9 corresponding to an element of timerInterruptHandlers
 */
timerInterruptIndex getTimerInterruptIndex(TIM_TypeDef* peripheral) {
    timerInterruptIndex interruptIdx;

    if (peripheral == TIM1) {
        interruptIdx = TIM1_IDX;
    } else if (peripheral == TIM2) {
        interruptIdx = TIM2_IDX;
    } else if (peripheral == TIM3) {
        interruptIdx = TIM3_IDX;
    } else if (peripheral == TIM4) {
        interruptIdx = TIM4_IDX;
    } else if (peripheral == TIM5) {
        interruptIdx = TIM5_IDX;
    } else if (peripheral == TIM9) {
        interruptIdx = TIM9_IDX;
    } else if (peripheral == TIM10) {
        interruptIdx = TIM10_IDX;
    } else if (peripheral == TIM11) {
        interruptIdx = TIM11_IDX;
    } else if (peripheral == TIM12) {
        interruptIdx = TIM12_IDX;
    } else if (peripheral == TIM13) {
        interruptIdx = TIM13_IDX;
    } else if (peripheral == TIM14) {
        interruptIdx = TIM14_IDX;
    } else if (peripheral == TIM8) {
        interruptIdx = TIM8_IDX;
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
        irqNum = TIM1_UP_TIM10_IRQn;
    } else if (peripheral == TIM2) {
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
    }  else if (peripheral == TIM8) {
        __HAL_RCC_TIM8_CLK_ENABLE();
        irqNum = TIM8_UP_TIM13_IRQn;
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
        irqNum = TIM1_UP_TIM10_IRQn;
    } else if (peripheral == TIM2) {
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
    } else if (peripheral == TIM8) {
        __HAL_RCC_TIM8_CLK_DISABLE();
        irqNum = TIM8_UP_TIM13_IRQn;
    } else {
        return; // Should never reach, but if an invalid peripheral is passed in then simply return
    }

    HAL_NVIC_DisableIRQ(irqNum);
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    uint8_t interruptIdx = getTimerInterruptIndex(htim->Instance);

    if (timerInterruptHandlers[interruptIdx] != nullptr) {
        timerInterruptHandlers[interruptIdx](htim);
    }
}

extern "C" void TIM1_UP_TIM10_IRQHandler(void) {
    if (const timerInterruptIndex tim1Index = getTimerInterruptIndex(TIM1); halTimers[tim1Index].Instance != nullptr) {
        HAL_TIM_IRQHandler(&halTimers[tim1Index]);
    }

    if (const timerInterruptIndex tim10Index = getTimerInterruptIndex(TIM10); halTimers[tim10Index].Instance != nullptr) {
        HAL_TIM_IRQHandler(&halTimers[tim10Index]);
    }
}

extern "C" void TIM8_UP_TIM13_IRQHandler(void) {
    if (const timerInterruptIndex tim8Index = getTimerInterruptIndex(TIM8); halTimers[tim8Index].Instance != nullptr) {
        HAL_TIM_IRQHandler(&halTimers[tim8Index]);
    }

    if (const timerInterruptIndex tim13Index = getTimerInterruptIndex(TIM13); halTimers[tim13Index].Instance != nullptr) {
        HAL_TIM_IRQHandler(&halTimers[tim13Index]);
    }
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

namespace core::dev {
TimerF4xx::TimerF4xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, TimerConfiguration configuration, uint32_t clockPrescaler)
    : configuration(configuration) {
    this->clockPeriod   = clockPeriod;
    this->halTimer = &halTimers[getTimerInterruptIndex(timerPeripheral)];
    initTimer(timerPeripheral, clockPeriod, clockPrescaler);
};

TimerF4xx::~TimerF4xx() = default;

void TimerF4xx::initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, uint32_t clockPrescaler) {
    this->clockPeriod = clockPeriod;
    auto& htim = halTimers[getTimerInterruptIndex(timerPeripheral)];

    htim.Instance       = timerPeripheral;
    // Allows period increments of 1 ms with max of 2^(32) ms.
    htim.Init.CounterMode       = this->configuration.counterMode;
    htim.Init.ClockDivision     = this->configuration.clockDivision;
    htim.Init.AutoReloadPreload = this->configuration.autoReloadPreload;

    if (clockPrescaler == AUTO_PRESCALER) {
        uint32_t prescaler    = SystemCoreClock / 1000;
        htim.Init.Prescaler   = prescaler - 1; // Sets f_CK_PSC to 1000 Hz
    } else {
        htim.Init.Prescaler = clockPrescaler;
    }

    this->clockPeriod     = clockPeriod;
    htim.Init.Period      = clockPeriod - 1;

    HAL_TIM_Base_Init(&htim);

    TIM_ClockConfigTypeDef clockConfig   = { };

    clockConfig.ClockSource = this->configuration.clockSource;
    HAL_TIM_ConfigClockSource(&htim, &clockConfig);

    TIM_MasterConfigTypeDef masterConfig = { };

    // Timers 9-14 are NOT master mode compatible, so waste of time to go through config
    if (getTimerInterruptIndex(timerPeripheral) < timerInterruptIndex::TIM9_IDX) {
        masterConfig.MasterOutputTrigger = this->configuration.masterOutputTrigger;
        masterConfig.MasterSlaveMode     = this->configuration.masterSlaveMode;
        HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);
    }


    core::log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Setup Timer");
};

void TimerF4xx::startTimer(void (*irqHandler)(void* htim)) {
    TIM_TypeDef* timerPeripheral = this->halTimer->Instance;

    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY) {
        stopTimer();
    }

    timerInterruptHandlers[getTimerInterruptIndex(timerPeripheral)] = irqHandler;
    startTimer();
}

void TimerF4xx::stopTimer() {
    HAL_TIM_Base_Stop_IT(this->halTimer);
}

void TimerF4xx::startTimer() {
    // If timer is not waiting to start, stop it
    if (halTimer->State != HAL_TIM_STATE_READY) {
        stopTimer(); // Stop timer in case it was already running
    }
    auto htim = this->halTimer;

    // Clear the interrupt flag so interrupt doesn't trigger immediately
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(htim);
}

void TimerF4xx::reloadTimer() {
    this->halTimer->Instance->CNT = 0; // Clear the Counter register to reset the timer
}

void TimerF4xx::setPeriod(uint32_t clockPeriod, uint32_t clockPrescaler) {
    stopTimer();
    initTimer(this->halTimer->Instance, clockPeriod, clockPrescaler);
}
}
