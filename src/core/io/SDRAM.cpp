#include <core/io/SDRAM.hpp>

namespace core::io {

SDRAM::SDRAM(void* memoryAddress, Pin* pins, const SDRAMInitConfig& initConfig,
             const SDRAMTimingConfig& timingConfig) : memoryAddress(memoryAddress), pins(pins),
             initConfig(initConfig), timingConfig(timingConfig) {}

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