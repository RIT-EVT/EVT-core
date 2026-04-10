#ifndef EVT_FMC_HPP
#define EVT_FMC_HPP

#include <core/io/GPIO.hpp>

#ifdef STM32F3xx
    #error Controlling SDRAM through an FMC is not supported on F3xx chips
#endif

namespace core::io {

/**
 * The FMC is a Flexible Memory Controller used for controlling various forms of memory, including external SDRAM,
 * NOR flash, NAND flash, and SRAM, the F3 series chips only support SRAM.
 */
class FMC {
public:
    /**
     * Represents the status of operation of the FMC
     *
     * This can be used to represent the state of the SDRAM or the
     * result of an FMC operation.
     */
    enum class Status {
        OK       = 0x00U,
        ERROR    = 0x01U,
        BUSY     = 0x02U,
        TIMEOUT  = 0x03U,
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
     * Initializes an FMC device for controlling memory
     */
    FMC(void* memoryAddress, FMCPinConfig pinConfig);

    virtual ~FMC() = default;

    void* memoryAddress;

protected:
    FMCPinConfig fmcPinConfig;
};

} // namespace core::io

#endif // EVT_FMC_HPP