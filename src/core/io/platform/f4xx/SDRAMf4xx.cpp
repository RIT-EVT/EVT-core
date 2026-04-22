#include <core/io/platform/f4xx/GPIOf4xx.hpp>
#include <core/io/platform/f4xx/SDRAMf4xx.hpp>

namespace core::io {

SDRAMf4xx::SDRAMf4xx(const FMC_SDRAM_TypeDef* sdramDevice, FMCPinConfig pinConfig, SDRAMInitConfig sdramInitConfig,
                     SDRAMTimingConfig sdramTimingConfig)
    : SDRAM((sdramInitConfig.sdBank == FMC_SDRAM_BANK1) ? reinterpret_cast<void*>(SDRAM_BANK1)
                                                        : reinterpret_cast<void*>(SDRAM_BANK2),
            pinConfig, sdramInitConfig, sdramTimingConfig),
      sdramDevice(const_cast<FMC_SDRAM_TypeDef*>(sdramDevice)), sdram(), sdramTiming() {
    InitHardware(pinConfig);

    // map the class init structs to the hal structs
    sdram.Instance                = this->sdramDevice;
    sdram.Init.SDBank             = sdramInitConfig.sdBank;
    sdram.Init.ColumnBitsNumber   = sdramInitConfig.columnBitsNumber;
    sdram.Init.RowBitsNumber      = sdramInitConfig.rowBitsNumber;
    sdram.Init.MemoryDataWidth    = sdramInitConfig.memoryDataWidth;
    sdram.Init.InternalBankNumber = sdramInitConfig.internalBankNumber;
    sdram.Init.WriteProtection    = sdramInitConfig.writeProtection;
    sdram.Init.ReadBurst          = sdramInitConfig.readBurst;
    sdram.Init.ReadPipeDelay      = sdramInitConfig.readPipeDelay;

    sdramTiming.LoadToActiveDelay    = sdramTimingConfig.loadToActiveDelay;
    sdramTiming.ExitSelfRefreshDelay = sdramTimingConfig.exitSelfRefreshDelay;
    sdramTiming.SelfRefreshTime      = sdramTimingConfig.selfRefreshTime;
    sdramTiming.RowCycleDelay        = sdramTimingConfig.rowCycleDelay;
    sdramTiming.WriteRecoveryTime    = sdramTimingConfig.writeRecoveryTime;
    sdramTiming.RPDelay              = sdramTimingConfig.rpDelay;
    sdramTiming.RCDDelay             = sdramTimingConfig.rcdDelay;

    HAL_SDRAM_Init(&sdram, &sdramTiming);
}

FMC::Status SDRAMf4xx::EnableWriteProtection() {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_WriteProtection_Enable(this->sdramDevice, this->sdramInitConfig.sdBank);

    return FMC::HALStatusToFMCStatus(halStatus);
}

FMC::Status SDRAMf4xx::DisableWriteProtection() {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_WriteProtection_Disable(this->sdramDevice, this->sdramInitConfig.sdBank);

    return FMC::HALStatusToFMCStatus(halStatus);
}

FMC::Status SDRAMf4xx::SendCommand(SDRAMCommandStruct* command, uint32_t timeout) {
    FMC_SDRAM_CommandTypeDef halCommand{};

    halCommand.AutoRefreshNumber      = command->AutoRefreshNumber;
    halCommand.CommandMode            = command->CommandMode;
    halCommand.CommandTarget          = command->CommandTarget;
    halCommand.ModeRegisterDefinition = command->ModeRegisterDefinition;

    HAL_StatusTypeDef halStatus = FMC_SDRAM_SendCommand(this->sdramDevice, &halCommand, timeout);

    return FMC::HALStatusToFMCStatus(halStatus);
}

FMC::Status SDRAMf4xx::ProgramRefreshRate(uint32_t refreshRate) {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_ProgramRefreshRate(this->sdramDevice, refreshRate);
    
    return FMC::HALStatusToFMCStatus(halStatus);
}

FMC::Status SDRAMf4xx::SetAutoRefreshNumber(uint32_t autoRefreshNumber) {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_SetAutoRefreshNumber(this->sdramDevice, autoRefreshNumber);

    return FMC::HALStatusToFMCStatus(halStatus);
}

uint32_t SDRAMf4xx::GetModeStatus() {
    return FMC_SDRAM_GetModeStatus(sdramDevice, sdramInitConfig.sdBank);
}

SDRAMf4xx::SDRAMInitConfig SDRAMf4xx::defaultSdramInitConfig() {
    SDRAMInitConfig config{};

    config.sdBank             = FMC_SDRAM_BANK1;
    config.columnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
    config.rowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
    config.memoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
    config.internalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    config.casLatency         = FMC_SDRAM_CAS_LATENCY_2;
    config.writeProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    config.sdClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
    config.readBurst          = FMC_SDRAM_RBURST_ENABLE;
    config.readPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

    return config;
};

SDRAMf4xx::SDRAMTimingConfig SDRAMf4xx::defaultSdramTimingConfig() {
    SDRAMTimingConfig config{};

    config.loadToActiveDelay    = NSToSdramClockCycles(LOAD_MODE_REGISTER_TO_ACTIVE_NS);
    config.exitSelfRefreshDelay = NSToSdramClockCycles(EXIT_SELF_REFRESH_DELAY_NS);
    config.selfRefreshTime      = NSToSdramClockCycles(SELF_REFRESH_TIME_NS);
    config.rowCycleDelay        = NSToSdramClockCycles(ROW_CYCLE_DELAY_NS);
    config.writeRecoveryTime    = NSToSdramClockCycles(RECOVERY_DELAY_NS);
    config.rpDelay              = NSToSdramClockCycles(ROW_PRECHARGE_DELAY_NS);
    config.rcdDelay             = NSToSdramClockCycles(ROW_TO_COLUMN_DELAY_NS);

    return config;
};

void* SDRAMf4xx::getSDRAMMemoryAddress() const {
    return memoryAddress; // This is a protected variable from the FMC class definition
}

void SDRAMf4xx::InitHardware(const FMCPinConfig& pinConfig) {
    __HAL_RCC_FMC_CLK_ENABLE();

    InitPinGroup(pinConfig.address.pins, pinConfig.address.count);
    InitPinGroup(pinConfig.data.pins, pinConfig.data.count);
    InitPinGroup(pinConfig.byteEnable.pins, pinConfig.byteEnable.count);
    InitPinGroup(pinConfig.bank.pins, pinConfig.bank.count);
    InitPinGroup(pinConfig.command.pins, pinConfig.command.count);
}

void SDRAMf4xx::InitPinGroup(Pin* pins, uint8_t count) {
    GPIO_InitTypeDef gpioInit;

    GPIOf4xx::gpioStateInit(&gpioInit, pins, count, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
}

} // namespace core::io