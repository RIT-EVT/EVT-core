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

#include <core/io/SDRAM.hpp>
#include <HALf4/stm32f4xx_hal.h>

namespace core::io {

/**
 * Class initializes the FMC peripheral and associated GPIO pins,
 * configures SDRAM timing parameters, and provides simple SDRAM methods.
 */
class SDRAMf4xx : public SDRAM {
public:
    #define SDRAM_BANK1 0xC0000000
    #define SDRAM_BANK2 0xD0000000

    /**
     * Initializes an FMC device by enabling the specific peripheral clock,
     * setting up the SDRAM Controller
     *
     * @param[in] pins a struct containing an array of pins and their length for use by the SDRAM Controller.
     * @param[in] sdramInitConfig SDRAM controller configuration parameters.
     * @param[in] sdramTimingConfig SDRAM timing configuration parameters.
     *
     */
    SDRAMf4xx(SDRAMPinGroup& pins, const SDRAMInitConfig& sdramInitConfig,
              const SDRAMTimingConfig& sdramTimingConfig);

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
    Status SendCommand(SDRAMCommand type, SDRAMCommandTarget target, uint16_t refreshNumber, uint16_t modeRegister) override;

  /**
   * Program the SDRAM Memory Refresh rate.
   *
   * @param rowCount The number of rows in the SDRAM (1 << num_of_row_bits)
   * @param refreshTime The amount of time to do all refresh cycles
   * @return the result of attempting to program the refresh rate of the sdram
   */
  Status ProgramRefreshRate(uint32_t rowCount, uint32_t refreshTime) override;

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
    SDRAMState GetModeStatus() override;

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
    [[nodiscard]] void* getSDRAMMemoryAddress() const;

    /**
     * Helper function to initialize all GPIO SDRAM pins
     *
     * @param[in] pins a struct containing an array of all SDRAM GPIO pins and their length
     */
    static void InitHardware(SDRAMPinGroup& pins);

    FMC_SDRAM_TypeDef* sdramDevice;

    SDRAM_HandleTypeDef sdram;
    FMC_SDRAM_TimingTypeDef sdramTiming;
};

} // namespace core::io

#endif // EVT_SDRAMF4xx_HPP
