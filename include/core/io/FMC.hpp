#ifndef EVT_FMC_HPP
#define EVT_FMC_HPP

#include <core/io/GPIO.hpp>

namespace core::io {

class FMC {
public:
    /**
     * Holds all SDRAM controller settings that map directly to
     * the HAL_SDRAM_Init configuration structure.
     *
     * Must be initialized before passing into the constructor
     */
    struct SdramInitConfig {
        uint32_t sdBank;
        uint32_t columnBitsNumber;
        uint32_t rowBitsNumber;
        uint32_t memoryDataWidth;
        uint32_t internalBankNumber;
        uint32_t casLatency;
        uint32_t writeProtection;
        uint32_t sdClockPeriod;
        uint32_t readBurst;
        uint32_t readPipeDelay;
    };

    /**
     * Structure to simplify SDRAM timing initialization, contains all required SDRAM timing delays in clock cycles.
     *
     * Must be initialized before passing into the constructor
     */
    struct SdramTimingConfig {
        uint32_t loadToActiveDelay;
        uint32_t exitSelfRefreshDelay;
        uint32_t selfRefreshTime;
        uint32_t rowCycleDelay;
        uint32_t writeRecoveryTime;
        uint32_t rpDelay;
        uint32_t rcdDelay;
    };

    typedef Pin FMC_PIN;
    typedef FMC_PIN FMC_ADDRESS;
    typedef FMC_PIN FMC_DATA;
    typedef FMC_PIN FMC_BE;
    typedef FMC_PIN FMC_BANK;
    typedef FMC_PIN FMC_CMD;

    /**
     * Structure to hold an array of GPIO address pins for the FMC
     */
    struct FMCAddressPins {
        FMC_ADDRESS* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO address pins for the FMC
     */
    struct FMCDataPins {
        FMC_DATA* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO command pins for the FMC
     */
    struct FMCCommandPins {
        FMC_CMD* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO byte enable pins for the FMC
     */
    struct FMCByteEnablePins {
        FMC_BE* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO bank pins for the FMC
     */
    struct FMCBankPins {
        FMC_BANK* pins;
        uint8_t count;
    };

    /**
     * Groups all FMC GPIO pin configurations.
     *
     * Contains arrays of:
     * - Address pins
     * - Data pins
     * - Bank select pins
     * - Command pins
     * - Byte enable pins
     */
    struct FMCPinConfig {
        FMCAddressPins address;
        FMCDataPins data;
        FMCByteEnablePins byteEnable;
        FMCBankPins bank;
        FMCCommandPins command;
    };

    /**
     * Initializes an FMC device
     */
    FMC(void* sdramMemoryAddress, FMCPinConfig pinConfig, SdramInitConfig sdramInitConfig,
        SdramTimingConfig sdramTimingConfig);

    void* sdramMemoryAddress;

protected:
    FMCPinConfig fmcPinConfig;
    SdramInitConfig sdramInitConfig;
    SdramTimingConfig sdramTimingConfig;
};

} // namespace core::io

#endif // EVT_FMC_HPP
