#ifndef _EVT_STM32F302x8_
#define _EVT_STM32F302x8_

namespace EVT::core::platform {

// stm32f302x8_init() sets clock speed to 8 MHz
constexpr uint32_t CLK_SPEED = 8000000;

/**
 * Handles system level initialization of the STM32F302x8. This makes a
 * series of calls into the HAL to enable system peripherals and enable
 * all required clock.
 */
void stm32f302x8_init();

}  // namespace EVT::core::platform
#endif
