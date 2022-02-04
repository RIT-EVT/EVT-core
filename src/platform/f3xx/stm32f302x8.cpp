#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal.h>
#include <HALf3/stm32f3xx_it.h>

#include <EVT/platform/f3xx/stm32f302x8.hpp>

namespace EVT::core::platform {

void stm32f302x8_init() {
    HAL_Init();
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4; // PLL CLK at 8 * 4 = 32 MHz
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; // SYSCLK at 8 MHz (HSI CLK)
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

    // ADC is currently configured to run based on AHB clock.
    // Enable the PLL Clock below and update 'halADC.Init.ClockPrescaler' to run ADC using faster PLL clock
    /*
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC1;
    PeriphClkInit.Adc1ClockSelection = RCC_ADC1PLLCLK_DIV1;

    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
     */

    SysTick_Handler();
}

}// namespace EVT::core::platform
