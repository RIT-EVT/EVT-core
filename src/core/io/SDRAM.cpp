#include <core/io/SDRAM.hpp>

#ifdef STM32F4xx
    #include <HALf4/stm32f4xx_hal_rcc.h>
#endif
#ifdef STM32F3xx
    #include <HALf3/stm32f3xx_hal_rcc.h>
#endif


namespace core::io {

SDRAM::SDRAM(void* sdramMemoryAddress, FMCPinConfig pinConfig, SDRAMInitConfig sdramInitConfig, SDRAMTimingConfig sdramTimingConfig)
        : FMC(sdramMemoryAddress, pinConfig) ,sdramTimingConfig(sdramTimingConfig),  sdramInitConfig(sdramInitConfig) {}

uint32_t SDRAM::getSdramClockFrequency() {
    return HAL_RCC_GetSysClockFreq() / 2;
}

uint32_t SDRAM::getSdramClockPeriodUS() {
    return 1000000000UL / (getSdramClockFrequency() / 1000);
}

uint32_t SDRAM::NSToSdramClockCycles(uint32_t nanoseconds) {
    return (nanoseconds * 1000 + getSdramClockPeriodUS()) / (getSdramClockPeriodUS());
}
}

