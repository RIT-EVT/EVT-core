#ifndef EVT_FMC_HPP
#define EVT_FMC_HPP

#ifdef STM32F4xx
    #include "HALf4/stm32f4xx_hal.h"
    #include "HALf4/stm32f4xx_ll_fmc.h"
    #include "HALf4/stm32f4xx_hal_sdram.h"
#else
    #include "HALf3/stm32f3xx_hal.h"
    #include "HALf3/stm32f3xx_ll_fmc.h"
    #include "HALf3/stm32f3xx_hal_sdram.h"
#endif


#define SDRAM_TIMEOUT (0x0000FFFFUL)

typedef struct {
    uint16_t PIN;
    GPIO_TypeDef* PORT;
} GPIO;

typedef GPIO 		FMC_GPIO;
typedef FMC_GPIO 	FMC_ADDRESS;
typedef FMC_GPIO 	FMC_DATA;
typedef FMC_GPIO 	FMC_BE;
typedef FMC_GPIO 	FMC_BANK;
typedef FMC_GPIO 	FMC_CMD;

// ADDRESS PINS
#define FMC_A0 		((FMC_ADDRESS) 	{GPIO_PIN_0, GPIOF})
#define FMC_A1 		((FMC_ADDRESS) 	{GPIO_PIN_1, GPIOF})
#define FMC_A2 		((FMC_ADDRESS) 	{GPIO_PIN_2, GPIOF})
#define FMC_A3 		((FMC_ADDRESS) 	{GPIO_PIN_3, GPIOF})
#define FMC_A4 		((FMC_ADDRESS) 	{GPIO_PIN_4, GPIOF})
#define FMC_A5 		((FMC_ADDRESS) 	{GPIO_PIN_5, GPIOF})
#define FMC_A6 		((FMC_ADDRESS) 	{GPIO_PIN_12, GPIOF})
#define FMC_A7 		((FMC_ADDRESS)	{GPIO_PIN_13, GPIOF})
#define FMC_A8 		((FMC_ADDRESS) 	{GPIO_PIN_14, GPIOF})
#define FMC_A9 		((FMC_ADDRESS) 	{GPIO_PIN_15, GPIOF})
#define FMC_A10		((FMC_ADDRESS) 	{GPIO_PIN_0, GPIOG})
#define FMC_A11		((FMC_ADDRESS) 	{GPIO_PIN_1, GPIOG})

// DATA PINS
#define FMC_D0		((FMC_DATA) 	{GPIO_PIN_14, GPIOD})
#define FMC_D1		((FMC_DATA) 	{GPIO_PIN_15, GPIOD})
#define FMC_D2		((FMC_DATA) 	{GPIO_PIN_0, GPIOD})
#define FMC_D3		((FMC_DATA) 	{GPIO_PIN_1, GPIOD})
#define FMC_D4 		((FMC_DATA) 	{GPIO_PIN_7, GPIOE})
#define FMC_D5 		((FMC_DATA) 	{GPIO_PIN_8, GPIOE})
#define FMC_D6 		((FMC_DATA) 	{GPIO_PIN_9, GPIOE})
#define FMC_D7 		((FMC_DATA) 	{GPIO_PIN_10, GPIOE})
#define FMC_D8 		((FMC_DATA) 	{GPIO_PIN_11, GPIOE})
#define FMC_D9 		((FMC_DATA) 	{GPIO_PIN_12, GPIOE})
#define FMC_D10		((FMC_DATA) 	{GPIO_PIN_13, GPIOE})
#define FMC_D11		((FMC_DATA) 	{GPIO_PIN_14, GPIOE})
#define FMC_D12		((FMC_DATA) 	{GPIO_PIN_15, GPIOE})
#define FMC_D13		((FMC_DATA) 	{GPIO_PIN_8, GPIOD})
#define FMC_D14		((FMC_DATA) 	{GPIO_PIN_9, GPIOD})
#define FMC_D15		((FMC_DATA) 	{GPIO_PIN_10, GPIOD})

// BYTE ENABLE PINS
#define FMC_NBL0	((FMC_BE) 		{GPIO_PIN_0, GPIOE})
#define FMC_NBL1	((FMC_BE) 		{GPIO_PIN_1, GPIOE})

// BANK SELECT PINS
#define FMC_BA0		((FMC_BANK) 	{GPIO_PIN_4, GPIOG})
#define FMC_BA1		((FMC_BANK) 	{GPIO_PIN_5, GPIOG})

// COMMAND PINS
#define FMC_SDCLK	((FMC_CMD) 		{GPIO_PIN_8, GPIOG})
#define FMC_SDNCAS 	((FMC_CMD) 		{GPIO_PIN_15, GPIOG})
#define FMC_SDNE0 	((FMC_CMD) 		{GPIO_PIN_2, GPIOC})
#define FMC_SDCKE0 	((FMC_CMD) 		{GPIO_PIN_3, GPIOC})
#define FMC_SDNWE 	((FMC_CMD) 		{GPIO_PIN_5, GPIOH})
#define FMC_SDNRAS	((FMC_CMD) 		{GPIO_PIN_11, GPIOF})

// #define FMC_ 		((GPIO) {GPIO_PIN_x, GPIOx})

#define SDRAM_CLK_SPEED ((uint32_t)(HAL_RCC_GetSysClockFreq() / 2))
#define SDRAM_CLK_PERIOD_US (1000000000UL / (SDRAM_CLK_SPEED / 1000)) // LOSES SOME RESOLUTION, BUT IT'S NEEDED TO FIT WITHIN 32-BITS
#define NS_TO_SDRAM_CLK_CYCLES(NS) ((NS * 1000 + SDRAM_CLK_PERIOD_US) / (SDRAM_CLK_PERIOD_US))

// All of these can be found in the datasheet of the Ram chip
#define tRCD 							( 15 )
#define tRP 							( 15 )
#define tWR 							( 10 ) // only specifies two clock cycles
#define tRC 							( 63 )
#define tRAS 							( 42 )
#define tXSR 							( 70 )
#define tMRD 							( 10 ) // only specifies two clock cycles

// Specific names from the FMC
#define ROW_TO_COLUMN_DELAY_NS 			( tRCD )
#define ROW_PRECHARGE_DELAY_NS 			( tRP  )
#define RECOVERY_DELAY_NS 				( tWR  )
#define ROW_CYCLE_DELAY_NS 				( tRC  )
#define SELF_REFRESH_TIME_NS 			( tRAS )
#define EXIT_SELF_REFRESH_DELAY_NS 		( tXSR )
#define LOAD_MODE_REGISTER_TO_ACTIVE_NS ( tMRD )

// Converted to Clock_Cycles
#define ROW_TO_COLUMN_DELAY 			( NS_TO_SDRAM_CLK_CYCLES( tRCD ) ) //(tRCD * 1000 + SDRAM_CLK_PERIOD_US) / (SDRAM_CLK_PERIOD_US)
#define ROW_PRECHARGE_DELAY 			( NS_TO_SDRAM_CLK_CYCLES( tRP  ) )
#define RECOVERY_DELAY 					( NS_TO_SDRAM_CLK_CYCLES( tWR  ) )
#define ROW_CYCLE_DELAY 				( NS_TO_SDRAM_CLK_CYCLES( tRC  ) )
#define SELF_REFRESH_TIME 				( NS_TO_SDRAM_CLK_CYCLES( tRAS ) )
#define EXIT_SELF_REFRESH_DELAY 		( NS_TO_SDRAM_CLK_CYCLES( tXSR ) )
#define LOAD_MODE_REGISTER_TO_ACTIVE 	( NS_TO_SDRAM_CLK_CYCLES( tMRD ) )


#define	RAM_SIZE						(0x4000000) // 64 megabits
#define STARTING_ADDR					((uint32_t*)0xC000000)

namespace core::io {

class FMC {
public:
    /**
     * Structure to simplify SDRAM initialization, pre-filled with default values
     *
     * Holds all the parameters needed to configure the SDRAM
     */
    struct SdramInitConfig {
        FMC_SDRAM_TypeDef* sdramDevice = FMC_SDRAM_DEVICE;
        uint32_t sdBank = FMC_SDRAM_BANK1;
        uint32_t columnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
        uint32_t rowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
        uint32_t memoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
        uint32_t internalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
        uint32_t casLatency = FMC_SDRAM_CAS_LATENCY_2;
        uint32_t writeProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
        uint32_t sdClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
        uint32_t readBurst = FMC_SDRAM_RBURST_ENABLE;
        uint32_t readPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
    };

    /**
     * Structure to simplify SDRAM timing initialization, pre-filled with default values
     *
     * Holds all the parameters needed to configure the SDRAM timing
     */
    struct SdramTimingConfig {
        uint32_t loadToActiveDelay = LOAD_MODE_REGISTER_TO_ACTIVE;
        uint32_t exitSelfRefreshDelay = EXIT_SELF_REFRESH_DELAY;
        uint32_t selfRefreshTime = SELF_REFRESH_TIME;
        uint32_t rowCycleDelay = ROW_CYCLE_DELAY;
        uint32_t writeRecoveryTime = RECOVERY_DELAY;
        uint32_t rpDelay = ROW_PRECHARGE_DELAY;
        uint32_t rcdDelay = ROW_TO_COLUMN_DELAY;
    };

    /**
     * Structure to hold an array of GPIO address pins for the FMC
     */
    struct FMCAddressPins {
        FMC_ADDRESS *pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO address pins for the FMC
     */
    struct FMCDataPins {
        FMC_DATA *pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO command pins for the FMC
     */
    struct FMCCommandPins {
        FMC_CMD *pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO byte enable pins for the FMC
     */
    struct FMCByteEnablePins {
        FMC_BE *pins;
        uint8_t count;
    };

    /**
     * Structure to hold an array of GPIO bank pins for the FMC
     */
    struct FMCBankPins {
        FMC_BANK *pins;
        uint8_t count;
    };

    /**
     * Structure to hold all FMC GPIO pins
     */
    struct FMCPinConfig {
        FMCAddressPins address;
        FMCDataPins    data;
        FMCByteEnablePins byteEnable;
        FMCBankPins    bank;
        FMCCommandPins command;
    };

    /**
     * Initializes an FMC device
     *
     * @param[in] pin_config a struct containing all FMC GPIO pin group arrays
     * @param[in] sdramInitConfig a struct containing all FMC SDRAM configuration
     * @param[in] sdramTimingConfig a struct containing all FMC SDRAM timing configuration variables
     */
    FMC(const FMCPinConfig &pin_config, const SdramInitConfig& sdramInitConfig, const SdramTimingConfig& sdramTimingConfig);

    /**
     * Write a value to SDRAM at the specified byte offset.
     *
     * @param[in] offset Byte offset from the SDRAM base address
     * @param[in] value Value to write to memory
     */
    void write32(uint32_t offset, uint32_t value) const;

    /**
     * Read a value SDRAM at the specified byte offset.
     *
     * @param[in] offset Byte offset from the SDRAM base address
     */
    [[nodiscard]] uint32_t read32(uint32_t offset) const;

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
    void InitPinGroup(const FMC_GPIO* pins, uint8_t count);

    SdramInitConfig sdramInitConfig;
    SdramTimingConfig sdramTimingConfig;

    SDRAM_HandleTypeDef sdram;
    FMC_SDRAM_TimingTypeDef sdramTiming;

    uint32_t sdramMemoryAddress; //address to read and write to
};

}

#endif // EVT_FMC_HPP
