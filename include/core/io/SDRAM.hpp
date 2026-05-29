#ifndef EVT_SDRAM_HPP
#define EVT_SDRAM_HPP
#include <core/io/pin.hpp>

#ifdef STM32F4xx
    #include <HALf4/stm32f4xx_hal.h>

namespace core::io {
class SDRAMDevice;
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
     * Target for when sending an SDRAM Command
     */
    enum class SDRAMCommandTarget {
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
        PRECHARGE_ALL = 2,
        AUTO_REFRESH  = 3,
        SET_OPERATION = 4,
        SELF_REFRESH  = 5,
        POWER_DOWN    = 6,
    };

    /**
     * All the states that the SDRAM can be held in.
     * NORMAL_MODE means normal operation with nothing special happening
     * SELF_REFRESH_MODE means that all data cells are refreshed automatically, but the MCU does not know when
     * POWER_DOWN_MODE means the off state
     */
    enum class SDRAMState {
        NORMAL_MODE = 0,
        SELF_REFRESH_MODE,
        POWER_DOWN_MODE
    };

    /**
     * Holds all SDRAM controller settings that map directly to
     * the HAL_SDRAM_Init configuration structure.
     *
     * Values should be looked for in stm32f4xx_ll_fmc.h with macro prefix FMC_SDRAM...
     *
     * Must be initialized before passing into the constructor
     */
    struct SDRAMInitConfig {
        uint32_t sdBank; // Bank number for the controller (0 or 1 usually)
        uint32_t columnBitsNumber; // number of Horizontal Addressing Cells
        uint32_t rowBitsNumber; // number of Vertical Addressing Cells
        uint32_t memoryDataWidth; // How large the data is: 8, 16, or 32 bit
        uint32_t internalBankNumber; // How many layers of columns and rows there are. Usually 1, 2, or 4
        uint32_t casLatency; // How many SDRAM CLK Cycles from data fetch received to data available from the output
        uint32_t writeProtection; // If you want bank protection on at initialization
        uint32_t sdClockPeriod; // How many MCU controller clock cycles per SDRAM CLK Cycle. Usually 1, 2, or 3
        uint32_t readBurst; // How many bytes to expect per read request.
        uint32_t readPipeDelay; // Number of SDRAM CLK Cycles until data is available from read. Usually 1
    };

    /**
     * Structure to simplify SDRAM timing initialization, contains all required SDRAM timing delays in clock cycles.
     *
     * Must be initialized before passing into the constructor
     */
    struct SDRAMTimingConfig {
        uint32_t loadToActiveDelay; // Time to update the load/operation register to SDRAM being read for commands
        uint32_t exitSelfRefreshDelay; // How long to exit the self-refresh mode
        uint32_t selfRefreshTime; // SDRAM CLK Cycles a row will be unavailable for while refreshing
        uint32_t rowCycleDelay; // Number of SDRAM CLK Cycles until a new active command can be submitted to a bank
        uint32_t writeRecoveryTime; // SDRAM CLK Cycles from write until a precharge can be given
        uint32_t readToPrechargeDelay; // SDRAM CLK Cycles from read until a precharge
        uint32_t rcdDelay; // SDRAM CLK Cycles from an active to read/write
    };

    struct SDRAMPinGroup {
        Pin* pins;
        uint8_t numPins;
    };

    /**
     * Constructor for initializing an SDRAM to control external SDRAM
     *
     * @param memoryAddress first address of SDRAM memory
     * @param pins the pins for use by the SDRAM Controller
     * @param initConfig HAL-level SDRAM parameters for how initialization works
     * @param timingConfig HAL-level SDRAM parameters for properly orchestrating hardware timing
     * @param device interface class with abstract function that is overridden with a specific implementation
     */
    SDRAM(uint32_t* memoryAddress, SDRAMPinGroup& pins, const SDRAMInitConfig& initConfig,
          const SDRAMTimingConfig& timingConfig, const SDRAMDevice& device);

    /**
     * Gets the Frequency of the SDRAM CLK
     * @param mcuClkPerSdramClk Number of microcontroller clock cycles for every SDRAM Clock Cycle
     * @return the SDRAM clock frequency
     */
    static uint32_t getSdramClockFrequency(uint8_t mcuClkPerSdramClk);

    /**
     * Translates HAL SDRAM Clock number into a 2 or 3
     * @param sdClockPeriod HAL SDRAM Clock Number (see FMC_SDRAM_Clock_Period FMC SDRAM Clock Period)
     * @return Mcu Clock Cycles per SDRAM Clock Cycle
     */
    static uint8_t getMCUClkPerSdramClk(uint32_t sdClockPeriod);

    /**
     * Get how long one SDRAM Clock cycle is in femtoseconds
     * @param mcuClkPerSdramClk Number of microcontroller clock cycles for every SDRAM Clock Cycle
     * @return the SDRAM clock period in femtoseconds
     */
    static uint32_t getSdramClockPeriodFS(uint32_t mcuClkPerSdramClk);

    /**
     * Transform a time given in nanoseconds into how many clock cycles fit in that range
     * @param nanoseconds Number of nanoseconds
     * @param mcuClkPerSdramClk Number of microcontroller clock cycles for every SDRAM Clock Cycle
     * @return the SDRAM clock period in nanoseconds
     */
    static uint32_t NSToSdramClockCycles(uint32_t nanoseconds, uint8_t mcuClkPerSdramClk);

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
    virtual Status SendCommand(SDRAMCommand type, SDRAMCommandTarget target, uint16_t refreshNumber,
                               uint16_t modeRegister) = 0;

    /**
     * Program the SDRAM Memory Refresh rate.
     *
     * @param rowCount The number of rows in the SDRAM (1 << num_of_row_bits)
     * @param refreshTime The amount of time to do all refresh cycles
     * @return the result of attempting to program the refresh rate of the sdram
     */
    virtual Status ProgramRefreshRate(uint32_t rowCount, uint32_t refreshTime) = 0;

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

    [[nodiscard]] uint32_t* getSdramMemoryAddress() const {
        return this->memoryAddress;
    }

protected:
    /**
     * Starting address of the RAM
     */
    uint32_t* memoryAddress;
    /**
     * All the pins used by the RAM
     */
    SDRAMPinGroup& pins;
    /**
     * Base config of the SDRAM
     */
    SDRAMInitConfig initConfig;
    /**
     * Timing Config for the SDRAM Controller
     */
    SDRAMTimingConfig timingConfig;
    /**
     * Associated Device that holds all the commands necessary to start up the device
     */
    const SDRAMDevice& device;

    /**
     * Helper function to turn a HAL Status in than SDRAM::Status
     * @param hal_status
     * @return status returned by the HAL
     */
    static constexpr Status HALStatusToSDRAMStatus(uint32_t hal_status) {
        return static_cast<Status>(hal_status);
    }
};

/**
 * Interface class to force SDRAM realizations to implement sendStartUpCommands, so that on creation everything can
 * be made and handled at once. No waiting or calling extra functions after calling getSDRAM()
 */
class SDRAMDevice {
public:
    virtual ~SDRAMDevice()                                       = default;
    virtual SDRAM::Status sendStartUpCommands(SDRAM& controller) = 0;
};

} // namespace core::io

#endif // STM32F4xx

#endif // EVT_SDRAM_HPP
