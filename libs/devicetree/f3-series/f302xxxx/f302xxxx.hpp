#pragma once

#ifdef STM32F302x8

#if MCU_PIN_LETTER == K
    #define STM32F302Kxxx
#elif MCU_PIN_LETTER == C
    #define STM32F302Cxxx
#elif MCU_PIN_LETTER == R
    #define STM32F302Rxxx
#else
    #error "MCU_PIN_LETTER is not correct for this MCU_DEVICE"
#endif

#endif