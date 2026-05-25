#include <core/io/SDRAM.hpp>

namespace core::io {

    SDRAM::SDRAM(uint32_t* memoryAddress, SDRAMPinGroup& pins, const SDRAMInitConfig& initConfig,
                 const SDRAMTimingConfig& timingConfig, const SDRAMDevice& device)
        : memoryAddress(memoryAddress), pins(pins), initConfig(initConfig), timingConfig(timingConfig), device(device) {}

    uint32_t SDRAM::getSdramClockFrequency(uint8_t mcuClkPerSdramClk) {
        return HAL_RCC_GetSysClockFreq() / mcuClkPerSdramClk;
    }

    uint32_t SDRAM::getSdramClockPeriodFS(uint8_t mcuClkPerSdramClk) {
        return 1000000000UL / (getSdramClockFrequency(mcuClkPerSdramClk) / 1000000);

        /**
        1,000,000,000 / (getSdramClockFrequency(mcuClkPerSdramClk) / 1000000)
        1,000,000,000 / ((HAL_RCC_GetSysClockFreq() / mcuClkPerSdramClk) / 1000000
        Assume HAL_RCC_GetSysClockFreq() returns 100,000,000 Hz and mcuClkPerSdramClk = 2
        1,000,000,000 / ((100,000,000Hz / 2) / 1000000)
        1,000,000,000 / (50,000,000Hz / 1000000)
        1,000,000,000 / 50Hz   (1 / Hz = s)
        20,000,000 fs

        Why put 1 billion as the numerator? It is the largest base 10 number that would fit in a
        32-bit number. This gives maximum resolution for the following
        function that uses it and keeps the calculation clean.

        Why divide getSdramClockFrequency(...) by 1 million? This gives us how many megahertz
        the clock is running at, which is the most imporant part of the clock speed in our case.
        This makes the denominator of the outer division as small as possible, allowing us to get a high
        amount of accuracy for femtoseconds per SDRAM clock period
        */
    }

    uint32_t SDRAM::NSToSdramClockCycles(uint32_t nanoseconds, uint8_t mcuClkPerSdramClk) {
        return (nanoseconds * 1000000) / (getSdramClockPeriodFS(mcuClkPerSdramClk) + 1);
    } // Multiplied by 1,000,000 to get from nanoseconds to femtoseconds
} // namespace core::io