#include <core/io/platform/f4xx/FMCf4xx.hpp>
#include <core/io/FMC.hpp>

namespace core::io {

FMCf4xx::FMCf4xx(FMCPinConfig pinConfig, SdramInitConfig sdramInitConfig, SdramTimingConfig sdramTimingConfig) :
FMC(0xC0000000),
sdramInitConfig(sdramInitConfig),
sdramTimingConfig(sdramTimingConfig),
fmcPinConfig(pinConfig),
sdram({nullptr}),
sdramTiming({0}) {
    // FMC(getSdramBaseAddress());

    InitHardware(pinConfig);

    // map the class init structs to the hal structs
    sdram.Instance = sdramInitConfig.sdramDevice;
    sdram.Init.SDBank = sdramInitConfig.sdBank;
    sdram.Init.ColumnBitsNumber = sdramInitConfig.columnBitsNumber;
    sdram.Init.RowBitsNumber = sdramInitConfig.rowBitsNumber;
    sdram.Init.MemoryDataWidth = sdramInitConfig.memoryDataWidth;
    sdram.Init.InternalBankNumber = sdramInitConfig.internalBankNumber;
    sdram.Init.WriteProtection = sdramInitConfig.writeProtection;
    sdram.Init.ReadBurst = sdramInitConfig.readBurst;
    sdram.Init.ReadPipeDelay = sdramInitConfig.readPipeDelay;

    sdramTiming.LoadToActiveDelay = sdramTimingConfig.loadToActiveDelay;
    sdramTiming.ExitSelfRefreshDelay = sdramTimingConfig.exitSelfRefreshDelay;
    sdramTiming.SelfRefreshTime = sdramTimingConfig.selfRefreshTime;
    sdramTiming.RowCycleDelay = sdramTimingConfig.rowCycleDelay;
    sdramTiming.WriteRecoveryTime = sdramTimingConfig.writeRecoveryTime;
    sdramTiming.RPDelay = sdramTimingConfig.rpDelay;
    sdramTiming.RCDDelay = sdramTimingConfig.rcdDelay;

    HAL_SDRAM_Init(&sdram, &sdramTiming);
}

void FMCf4xx::write32(uint32_t offset, uint32_t value) const {
    // Ensure 4-byte alignment
    if (offset % sizeof(uint32_t) != 0)
        return;

    // Ensure within SDRAM bounds
    if (offset >= RAM_SIZE)
        return;

    auto* const ptr =
        reinterpret_cast<volatile uint32_t*>(sdramMemoryAddress + offset);

    *ptr = value;
}

uint32_t FMCf4xx::read32(uint32_t offset) const {
    // Ensure 4-byte alignment
    if (offset % sizeof(uint32_t) != 0)
        return 0;

    // Ensure within SDRAM bounds
    if (offset >= RAM_SIZE)
        return 0;

    auto* const ptr =
        reinterpret_cast<volatile uint32_t*>(sdramMemoryAddress + offset);

    return *ptr;
}

uint32_t FMCf4xx::getSdramBaseAddress() {
    if (sdramInitConfig.sdBank == FMC_SDRAM_BANK1) //determine read write memory address
        return 0xC0000000;

    return 0xD0000000; //else
}

void FMCf4xx::InitHardware(const FMCPinConfig& pinConfig) {
    __HAL_RCC_FMC_CLK_ENABLE();

    InitPinGroup(pinConfig.address.pins, pinConfig.address.count);
    InitPinGroup(pinConfig.data.pins, pinConfig.data.count);
    InitPinGroup(pinConfig.byteEnable.pins, pinConfig.byteEnable.count);
    InitPinGroup(pinConfig.bank.pins, pinConfig.bank.count);
    InitPinGroup(pinConfig.command.pins, pinConfig.command.count);
}

void FMCf4xx::InitPinGroup(const FMC_GPIO* pins, uint8_t count) {
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