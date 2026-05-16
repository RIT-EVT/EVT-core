#pragma once

#if not defined(MCU_DEVICE)
    #error "MCU_CORE is not defined"
#endif

#if not defined(MCU_PIN_LETTER)
    #error "MCU_PIN_LETTER is not defined"
#endif

#if   MCU_DEVICE == F302
    #define STM32F302xx
#elif MCU_DEVICE == F334
    #define STM32F334xx
#elif MCU_DEVICE == F446
    #define STM32F446xx
#elif MCU_DEVICE == F469
    #define STM32F469xx
#else
    #error "MCU_DEVICE is not supported by evt core)
#endif


#if MCU_PIN_NUMBER == 20
    #define PACKAGE F
#elif MCU_PIN_NUMBER == 28
    #define PACKAGE G
#elif MCU_PIN_NUMBER == 32
    #define PACKAGE K
#elif MCU_PIN_NUMBER == 36
    #define PACKAGE T
#elif MCU_PIN_NUMBER == 40
    #define PACKAGE H
#elif MCU_PIN_NUMBER == 44
    #define PACKAGE S
#elif MCU_PIN_NUMBER == 48
    #define PACKAGE C
#elif MCU_PIN_NUMBER == 64
    #define PACKAGE R
#elif MCU_PIN_NUMBER == 81
    #define PACKAGE M
#elif MCU_PIN_NUMBER == 100
    #define PACKAGE V
#elif MCU_PIN_NUMBER == 132
    #define PACKAGE Q
#elif MCU_PIN_NUMBER == 144
    #define PACKAGE Z
#elif MCU_PIN_NUMBER == 169 || MCU_PIN_NUMBER == 168
    #define PACKAGE A
#elif MCU_PIN_NUMBER == 176
    #define PACKAGE I
#elif MCU_PIN_NUMBER == 208
    #define PACKAGE B
#elif MCU_PIN_NUMBER == 216
    #define PACKAGE N
#endif