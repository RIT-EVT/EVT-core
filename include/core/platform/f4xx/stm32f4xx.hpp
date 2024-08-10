#ifndef _EVT_STM32F4xx_
#define _EVT_STM32F4xx_

namespace core::platform {

// stm32f4xx_init() sets clock speed to 8MHz
constexpr uint32_t CLK_SPEED = 8000000;

// Interrupt Priority Mappings. 0 is high and 15 is low
constexpr uint32_t CAN_INTERRUPT_PRIORITY = 4;
constexpr uint32_t ADC_INTERRUPT_PRIORITY = 5;
constexpr uint32_t TIMER_INTERRUPT_PRIORITY = 9;
constexpr uint32_t GPIO_INTERRUPT_PRIORITY = 10;

/**
 * Handles system level initialization of the STM32F4xx. This makes a
 * series of calls into the HAL to enable system peripherals and enable
 * all required clock.
 */
void stm32f4xx_init();
void Error_Handler(void);

}// namespace core::platform
#endif
