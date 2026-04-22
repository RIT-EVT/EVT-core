#ifndef EVT_SDRAMF4xx_HPP
#define EVT_SDRAMF4xx_HPP

/**
 * FMC SDRAM driver for STM32F4 series.
 *
 * Provides a C++ abstraction for configuring and accessing external SDRAM
 * using the Flexible Memory Controller (FMC) peripheral.
 *
 * This driver wraps STM32 HAL functionality and simplifies:
 * - GPIO configuration for FMC pins
 * - SDRAM timing configuration
 * - SDRAM operations
 */

#include <core/io/FMC.hpp>
#include <core/io/SDRAM.hpp>

#include <HALf4/stm32f4xx_hal.h>

namespace core::io {

// default timer values
constexpr uint32_t tRCD = 15;
constexpr uint32_t tRP  = 15;
constexpr uint32_t tWR  = 22; // only specifies two clock cycles
constexpr uint32_t tRC  = 63;
constexpr uint32_t tRAS = 42;
constexpr uint32_t tXSR = 70;
constexpr uint32_t tMRD = 22; // only specifies two clock cycles

// Specific names from the FMC
constexpr uint32_t ROW_TO_COLUMN_DELAY_NS          = tRCD;
constexpr uint32_t ROW_PRECHARGE_DELAY_NS          = tRP;
constexpr uint32_t RECOVERY_DELAY_NS               = tWR;
constexpr uint32_t ROW_CYCLE_DELAY_NS              = tRC;
constexpr uint32_t SELF_REFRESH_TIME_NS            = tRAS;
constexpr uint32_t EXIT_SELF_REFRESH_DELAY_NS      = tXSR;
constexpr uint32_t LOAD_MODE_REGISTER_TO_ACTIVE_NS = tMRD;

constexpr uint32_t SDRAM_TIMEOUT = 0x0000FFFFUL;
constexpr uint32_t RAM_SIZE      = 0x4000000; // 64 megabits
constexpr uint32_t SDRAM_BANK1   = 0xC0000000;
constexpr uint32_t SDRAM_BANK2   = 0xD0000000;

/**
 * Class initializes the FMC peripheral and associated GPIO pins,
 * configures SDRAM timing parameters, and provides simple SDRAM methods.
 */
class SDRAMf4xx : public SDRAM {
public:
    /**
     * Initializes an FMC device by enabling the specific peripheral clock,
     * setting up the SDRAM Controller
     *
     * @param[in] sdramDevice FMC_SDRAM Peripheral configuration registers location
     * @param[in] pinConfig All FMC GPIO pin configurations.
     * @param[in] sdramInitConfig SDRAM controller configuration parameters.
     * @param[in] sdramTimingConfig SDRAM timing configuration parameters.
     *
     */
    SDRAMf4xx(const FMC_SDRAM_TypeDef* sdramDevice, FMCPinConfig pinConfig, SDRAMInitConfig sdramInitConfig,
              SDRAMTimingConfig sdramTimingConfig);

    /**
     * Enable write protection for the sdram
     *
     * @return the result of attempting to enable the write protection
     */
    Status EnableWriteProtection() override;

    /**
     * Disable write protection for the sdram
     *
     * @return the result of attempting to disable the write protection
     */
    Status DisableWriteProtection() override;

    /**
      * @brief Send a command to the sdram
      * @param type is the kind of the command to be sent, can be a value from enum SDRAM::Command
      * @param target specifies which device to send the command to, can be a value from SDRAM::Bank
      * @param refreshNumber defines the number of SDRAM clock cycles where the controller sends the auto refresh
      *  command (essentially a halted state where the SDRAM will ignore any requests), can be a value between 1 and 15
      * @param modeRegister defines how the SDRAM will operate when sending the SET_SDRAM_MODE command, ignored
      *  when sending any other command. The specific value to send depends on the datasheet usually
      *  under Mode Register Definition
      * @return the result of attempting to send a command to the sdram
      */
    Status SendCommand(SDRAMCommand type, SDRAMBank target, uint16_t refreshNumber, uint16_t modeRegister) override;

    /**
     * Program the SDRAM Memory Refresh rate.
     *
     * @param refreshRate The SDRAM refresh rate value
     * @return the result of attempting to program the refresh rate of the sdram
     */
    Status ProgramRefreshRate(uint32_t refreshRate) override;

    /**
     * Force a number of Refresh Commands to the SDRAM, effectively making it idle.
     *
     * @param autoRefreshNumber Specifies the auto Refresh number.
     * @return STATUS::OK
     */
    Status SetAutoRefreshNumber(uint32_t autoRefreshNumber) override;

    /**
     * Returns the indicated FMC SDRAM bank mode status.
     *
     * @return The FMC SDRAM bank mode status, could be one of the following HAL defines:
     *         FMC_SDRAM_NORMAL_MODE, FMC_SDRAM_SELF_REFRESH_MODE or
     *         FMC_SDRAM_POWER_DOWN_MODE.
     */
    uint32_t GetModeStatus() override;

    /**
     * Returns a SdramInitConfig struct pre-filled with default values.
     * Intended to be overridden to suit the specific use case before being passed into the constructor.
     */
    static SDRAMInitConfig defaultSdramInitConfig();

    /**
     * Returns a SdramTimingConfig pre-filled with default values.
     * Intended to be overridden to suit the specific use case before being passed into the constructor.
     */
    static SDRAMTimingConfig defaultSdramTimingConfig();

private:
    /**
     * Helper function to determine the memory address based on the bank number
     */
    void* getSDRAMMemoryAddress() const;
    /**
     * Helper function to initialize all GPIO FMC pins
     *
     * @param[in] pins a struct containing all FMC GPIO pin group arrays
     */
    void InitHardware(const FMCPinConfig& pins);

    /**
     * Helper function to initialize a single GPIO FMC pin group
     *
     * @param[in] pins an array of FMC GPIO pins
     * @param[in] count length of array
     */
    void InitPinGroup(Pin* pins, uint8_t count);

    FMC_SDRAM_TypeDef* sdramDevice;

    SDRAM_HandleTypeDef sdram;
    FMC_SDRAM_TimingTypeDef sdramTiming;
};

} // namespace core::io

#endif // EVT_SDRAMF4xx_HPP
