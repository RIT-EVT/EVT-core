#include <core/io/platform/f4xx/FMCf4xx.hpp>
#include <core/io/platform/f4xx/GPIOf4xx.hpp>

namespace core::io {

FMCf4xx::FMCf4xx(FMC_SDRAM_TypeDef* sdramDevice, FMCPinConfig pinConfig, SdramInitConfig sdramInitConfig, SdramTimingConfig sdramTimingConfig) :
FMC(sdramInitConfig.sdBank == FMC_SDRAM_BANK1 ? STARTING_ADDR : ALT_STARTING_ADDR, pinConfig, sdramInitConfig, sdramTimingConfig),
sdramDevice(sdramDevice),
sdram({nullptr}),
sdramTiming({0}) {
    InitHardware(pinConfig);

    // map the class init structs to the hal structs
    sdram.Instance                   = sdramDevice;
    sdram.Init.SDBank                = sdramInitConfig.sdBank;
    sdram.Init.ColumnBitsNumber      = sdramInitConfig.columnBitsNumber;
    sdram.Init.RowBitsNumber         = sdramInitConfig.rowBitsNumber;
    sdram.Init.MemoryDataWidth       = sdramInitConfig.memoryDataWidth;
    sdram.Init.InternalBankNumber    = sdramInitConfig.internalBankNumber;
    sdram.Init.WriteProtection       = sdramInitConfig.writeProtection;
    sdram.Init.ReadBurst             = sdramInitConfig.readBurst;
    sdram.Init.ReadPipeDelay         = sdramInitConfig.readPipeDelay;

    sdramTiming.LoadToActiveDelay    = sdramTimingConfig.loadToActiveDelay;
    sdramTiming.ExitSelfRefreshDelay = sdramTimingConfig.exitSelfRefreshDelay;
    sdramTiming.SelfRefreshTime      = sdramTimingConfig.selfRefreshTime;
    sdramTiming.RowCycleDelay        = sdramTimingConfig.rowCycleDelay;
    sdramTiming.WriteRecoveryTime    = sdramTimingConfig.writeRecoveryTime;
    sdramTiming.RPDelay              = sdramTimingConfig.rpDelay;
    sdramTiming.RCDDelay             = sdramTimingConfig.rcdDelay;

    HAL_SDRAM_Init(&sdram, &sdramTiming);
}

FMC::SdramInitConfig defaultSdramInitConfig() {
    FMC::SdramInitConfig config {};

    config.sdBank = FMC_SDRAM_BANK1;
    config.columnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
    config.rowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
    config.memoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
    config.internalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    config.casLatency = FMC_SDRAM_CAS_LATENCY_2;
    config.writeProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    config.sdClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
    config.readBurst = FMC_SDRAM_RBURST_ENABLE;
    config.readPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;

    return config;
};

FMC::SdramTimingConfig defaultSdramTimingConfig() {
    FMC::SdramTimingConfig config {};

    config.loadToActiveDelay = LOAD_MODE_REGISTER_TO_ACTIVE;
    config.exitSelfRefreshDelay = EXIT_SELF_REFRESH_DELAY;
    config.selfRefreshTime = SELF_REFRESH_TIME;
    config.rowCycleDelay = ROW_CYCLE_DELAY;
    config.writeRecoveryTime = RECOVERY_DELAY;
    config.rpDelay = ROW_PRECHARGE_DELAY;
    config.rcdDelay = ROW_TO_COLUMN_DELAY;

    return config;
};

void FMCf4xx::InitHardware(const FMCPinConfig& pinConfig) {
    __HAL_RCC_FMC_CLK_ENABLE();

    InitPinGroup(pinConfig.address.pins, pinConfig.address.count);
    InitPinGroup(pinConfig.data.pins, pinConfig.data.count);
    InitPinGroup(pinConfig.byteEnable.pins, pinConfig.byteEnable.count);
    InitPinGroup(pinConfig.bank.pins, pinConfig.bank.count);
    InitPinGroup(pinConfig.command.pins, pinConfig.command.count);
}

void FMCf4xx::InitPinGroup(FMC_PIN* pins, uint8_t count) {
    GPIO_InitTypeDef gpioInit;

    GPIOf4xx::gpioStateInit(&gpioInit, pins, count, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
}

} // namespace core::io