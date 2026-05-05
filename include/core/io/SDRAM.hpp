#ifndef EVT_SDRAM_HPP
#define EVT_SDRAM_HPP
#include <core/io/pin.hpp>

#ifdef STM32F4xx
    #include <HALf4/stm32f4xx_hal.h>

namespace core::io {

/**
 * Interface for configuring and accessing external SDRAM.
 * Provides clock frequency functions
 */
class SDRAM {
public:
    /**
     * Represents the status of operation of the SDRAM
     *
     * This can be used to represent the overall state of the SDRAM
     */
    enum class Status {
        OK      = 0x00U,
        ERROR   = 0x01U,
        BUSY    = 0x02U,
        TIMEOUT = 0x03U,
    };

    /**
     * SDRAM Command Target
     */
    enum class SDRAMBank {
        BANK1 = FMC_SDCMR_CTB1,
        BANK2 = FMC_SDCMR_CTB2,
        BOTH  = FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2,
    };

    /**
     * SDRAM Command modes for initialization and state change
     */
    enum class SDRAMCommand {
        NORMAL        = 0,
        CLK_ENABLE    = 1,
        PALL          = 2,
        AUTO_REFRESH  = 3,
        SET_OPERATION = 4,
        SELF_REFRESH  = 5,
        POWER_DOWN    = 6,
    };

    enum class SDRAMState {
        NORMAL_MODE = 0,
        SELF_REFRESH_MODE,
        POWER_DOWN_MODE
    };

    /**
     * Holds all SDRAM controller settings that map directly to
     * the HAL_SDRAM_Init configuration structure.
     *
     * Must be initialized before passing into the constructor
     */
    struct SDRAMInitConfig {
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
    struct SDRAMTimingConfig {
        uint32_t loadToActiveDelay;
        uint32_t exitSelfRefreshDelay;
        uint32_t selfRefreshTime;
        uint32_t rowCycleDelay;
        uint32_t writeRecoveryTime;
        uint32_t rpDelay;
        uint32_t rcdDelay;
    };

    /**
     * Constructor for initializing an SDRAM to control external SDRAM
     *
     * @param memoryAddress first address of SDRAM memory
     * @param pins the pins for use by the SDRAM Controller
     * @param initConfig HAL-level SDRAM parameters for how initialization works
     * @param timingConfig HAL-level SDRAM parameters for properly orchestrating hardware timing
     */
    SDRAM(void* memoryAddress, Pin* pins, const SDRAMInitConfig& initConfig, const SDRAMTimingConfig& timingConfig);

    /**
     * Gets the Frequency of the SDRAM CLK
     *
     * @return the SDRAM clock frequency
     */
    static uint32_t getSdramClockFrequency();

    /**
     * Get how long one SDRAM Clock cycle is in picoseconds
     *
     * @return the SDRAM clock period in picoseconds
     */
    static uint32_t getSdramClockPeriodPS();

    /**
     * Transform a time given in nanoseconds into how many clock cycles fit in that range
     *
     * @return the SDRAM clock period in nanoseconds
     */
    static uint32_t NSToSdramClockCycles(uint32_t nanoseconds);

    /**
     * Enable write protection for the sdram
     *
     * @return the result of attempting to enable the write protection
     */
    virtual Status EnableWriteProtection() = 0;

    /**
     * Disable write protection for the sdram
     *
     * @return the result of attempting to disable the write protection
     */
    virtual Status DisableWriteProtection() = 0;

    /**
     * @brief Send a command to the sdram
     *
     * @param type is the kind of the command to be sent, can be a value from enum SDRAM::Command
     * @param target specifies which device to send the command to, can be a value from SDRAM::Bank
     * @param refreshNumber defines the number of SDRAM clock cycles where the controller sends the auto refresh
     *  command (essentially a halted state where the SDRAM will ignore any requests), can be a value between 1 and 15
     * @param modeRegister defines how the SDRAM will operate when sending the SET_OPERATION command, ignored
     *  when sending any other command. The specific value to send depends on the datasheet usually
     *  under Mode Register Definition
     * @return the result of attempting to send a command to the sdram
     */
    virtual Status SendCommand(SDRAMCommand type, SDRAMBank target, uint16_t refreshNumber, uint16_t modeRegister) = 0;

    /**
     * Program the SDRAM Memory Refresh rate.
     *
     * @param refreshRate The SDRAM refresh rate value
     * @return the result of attempting to program the refresh rate of the sdram
     */
    virtual Status ProgramRefreshRate(uint32_t refreshRate) = 0;

    /**
     * Force a number of Refresh Commands to the SDRAM, effectively making it idle.
     *
     * @param autoRefreshNumber Specifies the auto Refresh number.
     * @return
     */
    virtual Status SetAutoRefreshNumber(uint32_t autoRefreshNumber) = 0;

    /**
     * Returns the indicated SDRAM bank mode status.
     *
     * @return The SDRAM bank mode status, could be on of the following HAL defines:
     *         FMC_SDRAM_NORMAL_MODE, FMC_SDRAM_SELF_REFRESH_MODE or
     *         FMC_SDRAM_POWER_DOWN_MODE.
     */
    virtual SDRAMState GetModeStatus() = 0;

    virtual ~SDRAM() = default;

    [[nodiscard]] void* getSdramMemoryAddress() const {
        return this->memoryAddress;
    }

protected:
    void* memoryAddress;
    Pin* pins;
    SDRAMInitConfig initConfig;
    SDRAMTimingConfig timingConfig;

    static constexpr Status HALStatusToSDRAMStatus(uint32_t hal_status) {
        return static_cast<Status>(hal_status);
    }
};

} // namespace core::io

#endif // STM32F4xx

#endif // EVT_SDRAM_HPP
