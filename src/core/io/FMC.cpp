#include <core/io/FMC.hpp>

namespace core::io {

FMC::FMC(const FMCPinConfig& pin_config, const SdramInitConfig& sdramInitConfig, const SdramTimingConfig& sdramTimingConfig) :
sdramDevice(*sdramInitConfig.sdramDevice),
sdBank(sdramInitConfig.sdBank),
columnBitsNumber(sdramInitConfig.columnBitsNumber),
rowBitsNumber(sdramInitConfig.rowBitsNumber),
memoryDataWidth(sdramInitConfig.memoryDataWidth),
internalBankNumber(sdramInitConfig.internalBankNumber),
casLatency(sdramInitConfig.casLatency),
writeProtection(sdramInitConfig.writeProtection),
sdClockPeriod(sdramInitConfig.sdClockPeriod),
readBurst(sdramInitConfig.readBurst),
readPipeDelay(sdramInitConfig.readPipeDelay),
loadToActiveDelay(sdramTimingConfig.loadToActiveDelay),
exitSelfRefreshDelay(sdramTimingConfig.exitSelfRefreshDelay),
selfRefreshTime(sdramTimingConfig.selfRefreshTime),
rowCycleDelay(sdramTimingConfig.rowCycleDelay),
writeRecoveryTime(sdramTimingConfig.writeRecoveryTime),
rpDelay(sdramTimingConfig.rpDelay),
rcdDelay(sdramTimingConfig.rcdDelay) {
    InitHardware(pin_config);

    sdram = {0};
    sdramTiming = {0};

    // map the class init structs to the hal structs
    sdram.Instance = sdramInitConfig.sdramDevice;
    sdram.Init.SDBank = sdramInitConfig.sdBank;
    sdram.Init.ColumnBitsNumber = columnBitsNumber;
    sdram.Init.RowBitsNumber = rowBitsNumber;
    sdram.Init.MemoryDataWidth = memoryDataWidth;
    sdram.Init.InternalBankNumber = internalBankNumber;
    sdram.Init.WriteProtection = writeProtection;
    sdram.Init.ReadBurst = readBurst;
    sdram.Init.ReadPipeDelay = readPipeDelay;

    sdramTiming.LoadToActiveDelay = loadToActiveDelay;
    sdramTiming.ExitSelfRefreshDelay = exitSelfRefreshDelay;
    sdramTiming.SelfRefreshTime = selfRefreshTime;
    sdramTiming.RowCycleDelay = rowCycleDelay;
    sdramTiming.WriteRecoveryTime = writeRecoveryTime;
    sdramTiming.RPDelay = rpDelay;
    sdramTiming.RCDDelay = rcdDelay;

    HAL_SDRAM_Init(&sdram, &sdramTiming);

    if (sdBank == FMC_SDRAM_BANK1)
        baseAddress = 0xC0000000;
    else
        baseAddress = 0xD0000000;
}

void FMC::write32(uint32_t offset, uint32_t value) const
{
    volatile uint32_t* ptr =
        reinterpret_cast<volatile uint32_t*>(baseAddress + offset);

    *ptr = value;
}

uint32_t FMC::read32(uint32_t offset) const
{
    volatile uint32_t* ptr =
        reinterpret_cast<volatile uint32_t*>(baseAddress + offset);

    return *ptr;
}

void FMC::InitHardware(const FMCPinConfig& pin_config) {
    __HAL_RCC_FMC_CLK_ENABLE();

    InitPinGroup(pin_config.address.pins, pin_config.address.count);
    InitPinGroup(pin_config.data.pins, pin_config.data.count);
    InitPinGroup(pin_config.byteEnable.pins, pin_config.byteEnable.count);
    InitPinGroup(pin_config.bank.pins, pin_config.bank.count);
    InitPinGroup(pin_config.command.pins, pin_config.command.count);
}

void FMC::InitPinGroup(const FMC_GPIO* pins, uint8_t count) {
    GPIO_InitTypeDef gpio {};

    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF12_FMC;

    for (uint8_t i = 0; i < count; i++)
    {
        gpio.Pin = pins[i].PIN;

        if (pins[i].PORT == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
        else if (pins[i].PORT == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
        else if (pins[i].PORT == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
        else if (pins[i].PORT == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
        else if (pins[i].PORT == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
        else if (pins[i].PORT == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();
        else if (pins[i].PORT == GPIOG) __HAL_RCC_GPIOG_CLK_ENABLE();
        else if (pins[i].PORT == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();

        HAL_GPIO_Init(pins[i].PORT, &gpio);
    }
}

} // namespace core::io