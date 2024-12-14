#include <../libs/HALf4/include/HALf4/stm32f4xx_hal.h>
#include <../libs/HALf4/include/HALf4/stm32f4xx_hal_tim.h>
#include <core/dev/platform/f4xx/Timerf4xx.hpp>
#include <core/rtos/BytePool.hpp>
#include <core/rtos/Threadx.hpp>
#include <core/utils/log.hpp>

namespace log = core::log;

namespace core::rtos {

namespace {
Initializable** initializableList;
size_t initListLength;
BytePoolBase* mainThreadPool;

TXError init(Initializable* initList[], std::size_t length, BytePoolBase& pool) {
    TXError errorCode = pool.init();
    if (errorCode != TXE_SUCCESS)
        return errorCode;

    return bulkInitialize(initList, length, pool);
}

extern "C" void tx_application_define(void* first_unused_memory) {
    init(initializableList, initListLength, *mainThreadPool);
}

} // namespace

TXError startKernel(Initializable* initList[], std::size_t length, BytePoolBase& pool) {
    initializableList = initList;
    initListLength    = length;
    mainThreadPool    = &pool;
    // tx_kernel_enter calls tx_application_define, which initializes everything in initList.
    tx_kernel_enter();
    return TXE_SUCCESS;
}

TXError bulkInitialize(Initializable** initList, std::size_t length, BytePoolBase& pool) {
    TXError errorCode = TXE_SUCCESS;
    for (std::size_t i = 0; i < length; i++) {
        errorCode = initList[i]->init(pool);
        if (errorCode != TXE_SUCCESS) {
            log::LOGGER.log(log::Logger::LogLevel::DEBUG,
                            "Errored on item %u in initializer list.\n\rError code: %u",
                            i,
                            errorCode);
            return errorCode;
        }
    }
    return errorCode;
}

void relinquish() {
    tx_thread_relinquish();
}

TXError sleep(uint32_t sleepTime) {
    uint32_t errorCode = tx_thread_sleep(sleepTime);
    return static_cast<TXError>(errorCode);
}

} // namespace core::rtos

/********************************************/
/**      SYSTICK OVERRIDE TO TIMER 6       **/
/********************************************/

TIM_HandleTypeDef htim6;

/**
 * @brief  This function configures the TIM6 as a time base source.
 *         The time source is configured  to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called  automatically at the beginning of program after
 *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
extern "C" HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler = 0U;

    uint32_t uwPrescalerValue = 0U;
    uint32_t pFLatency;
    HAL_StatusTypeDef status;

    /* Enable TIM6 clock */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
    /* Compute TIM6 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1) {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    } else {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM6 */
    htim6.Instance = TIM6;

    /* Initialize TIMx peripheral as follow:

    + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    htim6.Init.Period            = (1000000U / 1000U) - 1U;
    htim6.Init.Prescaler         = uwPrescalerValue;
    htim6.Init.ClockDivision     = 0;
    htim6.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&htim6);
    if (status == HAL_OK) {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&htim6);
        if (status == HAL_OK) {
            /* Enable the TIM6 global Interrupt */
            HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
            /* Configure the SysTick IRQ priority */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS)) {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            } else {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}

/**
 * @brief  Suspend Tick increment.
 * @note   Disable the tick increment by disabling TIM6 update interrupt.
 * @param  None
 * @retval None
 */
extern "C" void HAL_SuspendTick(void) {
    /* Disable TIM6 update Interrupt */
    __HAL_TIM_DISABLE_IT(&htim6, TIM_IT_UPDATE);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling TIM6 update interrupt.
 * @param  None
 * @retval None
 */
extern "C" void HAL_ResumeTick(void) {
    /* Enable TIM6 Update interrupt */
    __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE);
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
}

/**
 * @brief This function handles TIM6 global interrupt and DAC1, DAC2 underrun error interrupts.
 */
extern "C" void TIM6_DAC_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim6);
}
