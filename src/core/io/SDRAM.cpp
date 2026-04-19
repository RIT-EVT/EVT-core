#include <core/io/SDRAM.hpp>

namespace core::io {

SDRAM::SDRAM(void* sdramMemoryAddress, const FMCPinConfig& pinConfig, const SDRAMInitConfig& sdramInitConfig,
             const SDRAMTimingConfig& sdramTimingConfig)
    : FMC(sdramMemoryAddress, pinConfig), sdramInitConfig(sdramInitConfig), sdramTimingConfig(sdramTimingConfig) {}

uint32_t SDRAM::getSdramClockFrequency() {
    return HAL_RCC_GetSysClockFreq() / 2;
}

uint32_t SDRAM::getSdramClockPeriodPS() {
    return 1000000000UL / (getSdramClockFrequency() / 1000);
}

uint32_t SDRAM::NSToSdramClockCycles(uint32_t nanoseconds) {
    return (nanoseconds * 1000) / (getSdramClockPeriodPS() + 1);
}
} // namespace core::io