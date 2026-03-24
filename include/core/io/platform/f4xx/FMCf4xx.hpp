#ifndef EVT_FMCF4xx_HPP
#define EVT_FMCF4xx_HPP

/**
 * FMC SDRAM driver for STM32F4 series.
 *
 * Provides a C++ abstraction for configuring and accessing external SDRAM
 * using the Flexible Memory Controller (FMC) peripheral.
 *
 * This driver wraps STM32 HAL functionality and simplifies:
 * - GPIO configuration for FMC pins
 * - SDRAM timing configuration
 * - SDRAM read/write operations
 */

#include <HALf4/stm32f4xx_hal.h>
#include <HALf4/stm32f4xx_hal_sdram.h>
#include <HALf4/stm32f4xx_ll_fmc.h>

#include <core/io/FMC.hpp>
#include <core/io/GPIO.hpp>

namespace core::io {

#define SDRAM_TIMEOUT (0x0000FFFFUL)

#define SDRAM_CLK_SPEED ((uint32_t) (HAL_RCC_GetSysClockFreq() / 2))
#define SDRAM_CLK_PERIOD_US \
    (1000000000UL / (SDRAM_CLK_SPEED / 1000)) // LOSES SOME RESOLUTION, BUT IT'S NEEDED TO FIT WITHIN 32-BITS
#define NS_TO_SDRAM_CLK_CYCLES(NS) ((NS * 1000 + SDRAM_CLK_PERIOD_US) / (SDRAM_CLK_PERIOD_US))

// All of these can be found in the datasheet of the Ram chip
#define tRCD (15)
#define tRP  (15)
#define tWR  (10) // only specifies two clock cycles
#define tRC  (63)
#define tRAS (42)
#define tXSR (70)
#define tMRD (10) // only specifies two clock cycles

// Specific names from the FMC
#define ROW_TO_COLUMN_DELAY_NS          (tRCD)
#define ROW_PRECHARGE_DELAY_NS          (tRP)
#define RECOVERY_DELAY_NS               (tWR)
#define ROW_CYCLE_DELAY_NS              (tRC)
#define SELF_REFRESH_TIME_NS            (tRAS)
#define EXIT_SELF_REFRESH_DELAY_NS      (tXSR)
#define LOAD_MODE_REGISTER_TO_ACTIVE_NS (tMRD)

// Converted to Clock_Cycles
#define ROW_TO_COLUMN_DELAY          (NS_TO_SDRAM_CLK_CYCLES(tRCD)) //(tRCD * 1000 + SDRAM_CLK_PERIOD_US) / (SDRAM_CLK_PERIOD_US)
#define ROW_PRECHARGE_DELAY          (NS_TO_SDRAM_CLK_CYCLES(tRP))
#define RECOVERY_DELAY               (NS_TO_SDRAM_CLK_CYCLES(tWR))
#define ROW_CYCLE_DELAY              (NS_TO_SDRAM_CLK_CYCLES(tRC))
#define SELF_REFRESH_TIME            (NS_TO_SDRAM_CLK_CYCLES(tRAS))
#define EXIT_SELF_REFRESH_DELAY      (NS_TO_SDRAM_CLK_CYCLES(tXSR))
#define LOAD_MODE_REGISTER_TO_ACTIVE (NS_TO_SDRAM_CLK_CYCLES(tMRD))

#define RAM_SIZE               (0x4000000) // 64 megabits
#define FMC_SDRAM_BANK1_BASE   ((uint32_t*) 0xC000000) // found in the reference manual for the MCU
#define FMC_SDRAM_BANK2_BASE   ((uint32_t*) 0xD000000)

/**
 * Driver for configuring and accessing external SDRAM via FMC.
 *
 * Class initializes the FMC peripheral and associated GPIO pins,
 * configures SDRAM timing parameters, and provides simple 32-bit
 * memory read/write access methods.
 */
class FMCf4xx : public FMC {
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
     * Initializes a FMC device
     *
     * @param[in] pinConfig All FMC GPIO pin configurations.
     * @param[in] sdramInitConfig SDRAM controller configuration parameters.
     * @param[in] sdramTimingConfig SDRAM timing configuration parameters.
     *
     * - Enables FMC peripheral clock
     * - Initializes GPIO pins
     * - Configures SDRAM controller
     * - Calls HAL_SDRAM_Init()
     */
    FMCf4xx(FMC_SDRAM_TypeDef* sdramDevice, FMCPinConfig pinConfig, SdramInitConfig sdramInitConfig,
            SdramTimingConfig sdramTimingConfig);

    /**
     * Enable write protection for the sdram
     *
     * @return the result of attempting to enable the write protection
     */
    Status WriteProtectionEnable();

    /**
     * Disable write protection for the sdram
     *
     * @return the result of attempting to disable the write protection
     */
    Status WriteProtection_Disable();

    /**
     * Send a command to the sdram
     *
     * @param command The SdramCommandStruct holding the information of the command
     * @param timeout How long to wait for the sdram command
     * @return the result of attempting to send a command to the sdram
     */
    Status SendCommand(FMC_SDRAM_CommandTypeDef *command, uint32_t timeout);

    /**
     * Program the SDRAM Memory Refresh rate.
     *
     * @param refreshRate The SDRAM refresh rate value
     * @return the result of attempting to program the refresh rate of the sdram
     */
    Status ProgramRefreshRate(uint32_t refreshRate);

    /**
     * Set the Number of consecutive SDRAM Memory auto Refresh commands.
     *
     * @param autoRefreshNumber Specifies the auto Refresh number.
     * @return
     */
    Status SetAutoRefreshNumber(uint32_t autoRefreshNumber);

    /**
     * Returns the indicated FMC SDRAM bank mode status.
     *
    * @return The FMC SDRAM bank mode status, could be on of the following HAL defines:
    *         FMC_SDRAM_NORMAL_MODE, FMC_SDRAM_SELF_REFRESH_MODE or
    *         FMC_SDRAM_POWER_DOWN_MODE.
     */
    uint32_t GetModeStatus();

    /**
     * Returns a SdramInitConfig struct pre-filled with default values.
     * Intended to be overridden to suit the specific use case before being passed into the constructor.
     */
    static SdramInitConfig defaultSdramInitConfig();

    /**
     * Returns a SdramTimingConfig pre-filled with default values.
     * Intended to be overridden to suit the specific use case before being passed into the constructor.
     */
    static SdramTimingConfig defaultSdramTimingConfig();

    void* sdramMemoryAddress;

private:
    /**
     * Helper function to determine the memory address based on the bank number
     *
     */
    uint32_t* getSDRAMMemoryAddress();
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
    void InitPinGroup(FMC_PIN* pins, uint8_t count);

    FMC_SDRAM_TypeDef* sdramDevice;

    FMCPinConfig fmcPinConfig;
    SdramInitConfig sdramInitConfig;
    SdramTimingConfig sdramTimingConfig;

    SDRAM_HandleTypeDef sdram;
    FMC_SDRAM_TimingTypeDef sdramTiming;
};

} // namespace core::io

#endif // EVT_FMCF4xx_HPP
