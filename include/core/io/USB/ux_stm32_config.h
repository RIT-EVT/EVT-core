//
// Created by oscarc on 10/1/26.
//

#ifndef CUI_UX_STM32_CONFIG_H
#define CUI_UX_STM32_CONFIG_H
#include <cstdint>

// threadx usbx library depends on this file existing and idk how to get around that
#if defined(STM32F4XX)
    #include "stm32f4xx_hal.h"
#elif defined(STM32F3XX)
    #include "stm32f3xx_hal.h"
#endif

constexpr uint8_t UX_DCD_STM32_MAX_ED = 8; // defines the maximum number of physical endpoints (ED) that the Device Controller Driver (DCD) will allocate memory for and manage

#endif // CUI_UX_STM32_CONFIG_H
