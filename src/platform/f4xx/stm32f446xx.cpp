#include <HALf4/stm32f4xx.h>
#include <HALf4/stm32f4xx_hal.h>
#include <HALf4/stm32f4xx_it.h>

#include <EVT/platform/f4xx/stm32f4xx.hpp>

namespace EVT::core::platform {

void stm32f4xx_init() {
    HAL_Init();
    
}
    
} // namespace EVT::core::platform
