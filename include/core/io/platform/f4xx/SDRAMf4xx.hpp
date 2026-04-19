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
 *
 *
 * Class initializes the FMC peripheral and associated GPIO pins,
 * configures SDRAM timing parameters, and provides simple SDRAM methods.
 */
class SDRAMf4xx : public SDRAM {
public:
    /**
     * Initializes an FMC device
     *
     * @param[in] sdramDevice FMC_SDRAM Peripheral configuration registers location
     * @param[in] pinConfig All FMC GPIO pin configurations.
     * @param[in] sdramInitConfig SDRAM controller configuration parameters.
     * @param[in] sdramTimingConfig SDRAM timing configuration parameters.
     *
     * - Enables FMC peripheral clock
     * - Initializes GPIO pins
     * - Configures SDRAM controller
     * - Calls HAL_SDRAM_Init()
     */
    SDRAMf4xx(const FMC_SDRAM_TypeDef* sdramDevice, FMCPinConfig pinConfig, SDRAMInitConfig sdramInitConfig,
              SDRAMTimingConfig sdramTimingConfig);

    /**
     * Enable write protection for the sdram
     *
     * @return the result of attempting to enable the write protection
     */
    Status WriteProtectionEnable() override;

    /**
     * Disable write protection for the sdram
     *
     * @return the result of attempting to disable the write protection
     */
    Status WriteProtection_Disable() override;

    /**
     * Send a command to the sdram
     *
     * @param command The SdramCommandStruct holding the information of the command
     * @param timeout How long to wait for the sdram command
     * @return the result of attempting to send a command to the sdram
     */
    Status SendCommand(SDRAMCommandStruct* command, uint32_t timeout) override;

    /**
     * Program the SDRAM Memory Refresh rate.
     *
     * @param refreshRate The SDRAM refresh rate value
     * @return the result of attempting to program the refresh rate of the sdram
     */
    Status ProgramRefreshRate(uint32_t refreshRate) override;

    /**
     * Set the Number of consecutive SDRAM Memory auto Refresh commands.
     *
     * @param autoRefreshNumber Specifies the auto Refresh number.
     * @return
     */
    Status SetAutoRefreshNumber(uint32_t autoRefreshNumber) override;

    /**
     * Returns the indicated FMC SDRAM bank mode status.
     *
     * @return The FMC SDRAM bank mode status, could be on of the following HAL defines:
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
     *
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
