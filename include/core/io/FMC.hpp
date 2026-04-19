#ifndef EVT_FMC_HPP
#define EVT_FMC_HPP

#include <core/io/GPIO.hpp>

//#ifdef STM32F3xx
//    #error Controlling SDRAM through an FMC is not supported on F3xx chips
//#endif

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

    /**
     * Structure to hold an array of GPIO address pins for the FMC
     */
    struct FMCAddressPins {
        Pin* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO address pins for the FMC
     */
    struct FMCDataPins {
        Pin* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO command pins for the FMC
     */
    struct FMCCommandPins {
        Pin* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO byte enable pins for the FMC
     */
    struct FMCByteEnablePins {
        Pin* pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO bank pins for the FMC
     */
    struct FMCBankPins {
        Pin* pins;
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
    FMC(void* memoryAddress, const FMCPinConfig& pinConfig);

    virtual ~FMC() = default;

    [[nodiscard]] void* getStartingAddress() const;

protected:
    FMCPinConfig fmcPinConfig;
    void* memoryAddress;
};

} // namespace core::io

#endif // EVT_FMC_HPP