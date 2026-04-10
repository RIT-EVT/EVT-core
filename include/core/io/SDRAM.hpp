#ifndef EVT_SDRAM_HPP
#define EVT_SDRAM_HPP

#include <core/io/FMC.hpp>

/**
 * SDRAM Command Mode
 */
#define FMC_SDRAM_CMD_NORMAL_MODE               (0x00000000U)
#define FMC_SDRAM_CMD_CLK_ENABLE                (0x00000001U)
#define FMC_SDRAM_CMD_PALL                      (0x00000002U)
#define FMC_SDRAM_CMD_AUTOREFRESH_MODE          (0x00000003U)
#define FMC_SDRAM_CMD_LOAD_MODE                 (0x00000004U)
#define FMC_SDRAM_CMD_SELFREFRESH_MODE          (0x00000005U)
#define FMC_SDRAM_CMD_POWERDOWN_MODE            (0x00000006U)

/**
 * SDRAM Command Target
 */
#define FMC_SDRAM_CMD_TARGET_BANK2              FMC_SDCMR_CTB2
#define FMC_SDRAM_CMD_TARGET_BANK1              FMC_SDCMR_CTB1
#define FMC_SDRAM_CMD_TARGET_BANK1_2            (0x00000018U)

namespace core::io {

/**
 * Interface for configuring and accessing external SDRAM via FMC.
 * Provides clock frequency functions
 */
class SDRAM : public FMC {
    public:
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
     * Structure to send commands to the SDRAM through the FMC
     *
     * CommandMode is the mode of the command, cam be a value from the SDRAM Command Mode #define group
     * CommandTarget specifies which device to send the command to, cam be a value from the SDRAM Command Target #define
     * group AutoRefreshNumber defines the number of consecutive auto refresh command issued in auto refresh mode, can
     * be a value between 1 and 15 ModeRegisterDefinition defines the SDRAM Mode to register content
     */
    struct SDRAMCommandStruct {
        uint32_t CommandMode;
        uint32_t CommandTarget;
        uint32_t AutoRefreshNumber;
        uint32_t ModeRegisterDefinition;
    };

    /**
     * Constructor for initializing an FMC to control external SDRAM
     *
     * @param sdramMemoryAddress
     * @param pinConfig
     * @param sdramInitConfig
     * @param sdramTimingConfig
     */
    SDRAM(void* sdramMemoryAddress, FMCPinConfig pinConfig, SDRAMInitConfig sdramInitConfig,
          SDRAMTimingConfig sdramTimingConfig);

    /**
     *
     * @return the SDRAM clock frequency
     */
    static uint32_t getSdramClockFrequency();

    /**
     *
     * @return the SDRAM clock period in microseconds
     */
    static uint32_t getSdramClockPeriodUS();

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
    virtual Status WriteProtectionEnable() = 0;

    /**
     * Disable write protection for the sdram
     *
     * @return the result of attempting to disable the write protection
     */
    virtual Status WriteProtection_Disable() = 0;

    /**
     * Send a command to the sdram
     *
     * @param command The SdramCommandStruct holding the information of the command
     * @param timeout How long to wait for the sdram command
     * @return the result of attempting to send a command to the sdram
     */
    virtual Status SendCommand(SDRAMCommandStruct *command, uint32_t timeout) = 0;

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

    void* sdramMemoryAddress;

protected:
    SDRAMInitConfig sdramInitConfig;
    SDRAMTimingConfig sdramTimingConfig;
};

}

#endif //EVT_SDRAM_HPP
