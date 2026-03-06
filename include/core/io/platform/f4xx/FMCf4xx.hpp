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
#include <HALf4/stm32f4xx_ll_fmc.h>
#include <HALf4/stm32f4xx_hal_sdram.h>

#include <core/io/FMC.hpp>
#include <core/io/GPIO.hpp>

namespace core::io {

#define SDRAM_TIMEOUT (0x0000FFFFUL)

#define SDRAM_CLK_SPEED ((uint32_t)(HAL_RCC_GetSysClockFreq() / 2))
#define SDRAM_CLK_PERIOD_US (1000000000UL / (SDRAM_CLK_SPEED / 1000)) // LOSES SOME RESOLUTION, BUT IT'S NEEDED TO FIT WITHIN 32-BITS
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


#define	RAM_SIZE						(0x4000000) // 64 megabits
#define STARTING_ADDR					((uint32_t*)0xC000000)
#define ALT_STARTING_ADDR               ((uint32_t*)0xD000000)
#define RAM_SIZE      (0x4000000) // 64 megabits
#define STARTING_ADDR ((uint32_t*) 0xC000000)

namespace core::io {

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
     * Initializes an FMC device
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
    FMCf4xx(FMCPinConfig pinConfig, SdramInitConfig sdramInitConfig, SdramTimingConfig sdramTimingConfig, FMC_SDRAM_TypeDef* sdramDevice);

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
private:
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

    SDRAM_HandleTypeDef sdram;
    FMC_SDRAM_TimingTypeDef sdramTiming;
};

} // namespace core::io

#endif // EVT_FMC_HPP
