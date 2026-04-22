#ifndef EVT_SDRAM_HPP
#define EVT_SDRAM_HPP

#ifdef STM32F4xx
    #include <HALf4/stm32f4xx_hal.h>
    #include <core/io/FMC.hpp>

namespace core::io {

/**
 * Interface for configuring and accessing external SDRAM via FMC.
 * Provides clock frequency functions
 */
class SDRAM : public FMC {
public:
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
        NORMAL_MODE      = 0,
        CLK_ENABLE       = 1,
        PALL             = 2,
        AUTOREFRESH_MODE = 3,
        SET_MODE         = 4,
        SELFREFRESH_MODE = 5,
        POWERDOWN_MODE   = 6,
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
     * Constructor for initializing an FMC to control external SDRAM
     *
     * @param sdramMemoryAddress
     * @param pinConfig
     * @param sdramInitConfig
     * @param sdramTimingConfig
     */
    SDRAM(void* sdramMemoryAddress, const FMCPinConfig& pinConfig, const SDRAMInitConfig& sdramInitConfig,
          const SDRAMTimingConfig& sdramTimingConfig);

    /**
     *
     * @return the SDRAM clock frequency
     */
    static uint32_t getSdramClockFrequency();

    /**
     *
     * @return the SDRAM clock period in microseconds
     */
    static uint32_t getSdramClockPeriodPS();

    /**
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
     * @param modeRegister defines how the SDRAM will operate when sending the SET_SDRAM_MODE command, ignored
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
     * Set the Number of consecutive SDRAM Memory auto Refresh commands.
     *
     * @param autoRefreshNumber Specifies the auto Refresh number.
     * @return
     */
    virtual Status SetAutoRefreshNumber(uint32_t autoRefreshNumber) = 0;

    /**
     * Returns the indicated FMC SDRAM bank mode status.
     *
     * @return The FMC SDRAM bank mode status, could be on of the following HAL defines:
     *         FMC_SDRAM_NORMAL_MODE, FMC_SDRAM_SELF_REFRESH_MODE or
     *         FMC_SDRAM_POWER_DOWN_MODE.
     */
    virtual uint32_t GetModeStatus() = 0;

protected:
    SDRAMInitConfig sdramInitConfig;
    SDRAMTimingConfig sdramTimingConfig;
};

} // namespace core::io

#endif // STM32F4xx

#endif // EVT_SDRAM_HPP
