#ifndef _EVT_STM32F302x8_
#define _EVT_STM32F302x8_

namespace EVT::core::platform {
/**
 * Handles system level initialization of the STM32F302x8. This makes a
 * series of calls into the HAL to enable system peripherals and enable
 * all required clock.
 */
void stm32f302x8_init();

}  // namespace EVT::core::platform
#endif