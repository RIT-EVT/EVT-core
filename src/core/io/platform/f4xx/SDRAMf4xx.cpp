#include <core/io/platform/f4xx/GPIOf4xx.hpp>
#include <core/io/platform/f4xx/SDRAMf4xx.hpp>

namespace core::io {

SDRAMf4xx::SDRAMf4xx(const FMC_SDRAM_TypeDef* sdramDevice, Pin* pins,
                     const SDRAMInitConfig& initConfig, const SDRAMTimingConfig& timingConfig)
                     : SDRAM((initConfig.sdBank == FMC_SDRAM_BANK1)
                              ? reinterpret_cast<void*>(SDRAM_BANK1)
                              : reinterpret_cast<void*>(SDRAM_BANK2),
                              pins, initConfig, timingConfig),
                     sdramDevice(const_cast<FMC_SDRAM_TypeDef*>(sdramDevice)), sdram(), sdramTiming() {
    InitHardware(pins);

    // map the class init structs to the hal structs
    sdram.Instance                = this->sdramDevice;
    sdram.Init.SDBank             = initConfig.sdBank;
    sdram.Init.ColumnBitsNumber   = initConfig.columnBitsNumber;
    sdram.Init.RowBitsNumber      = initConfig.rowBitsNumber;
    sdram.Init.MemoryDataWidth    = initConfig.memoryDataWidth;
    sdram.Init.InternalBankNumber = initConfig.internalBankNumber;
    sdram.Init.WriteProtection    = initConfig.writeProtection;
    sdram.Init.ReadBurst          = initConfig.readBurst;
    sdram.Init.ReadPipeDelay      = initConfig.readPipeDelay;

    sdramTiming.LoadToActiveDelay    = timingConfig.loadToActiveDelay;
    sdramTiming.ExitSelfRefreshDelay = timingConfig.exitSelfRefreshDelay;
    sdramTiming.SelfRefreshTime      = timingConfig.selfRefreshTime;
    sdramTiming.RowCycleDelay        = timingConfig.rowCycleDelay;
    sdramTiming.WriteRecoveryTime    = timingConfig.writeRecoveryTime;
    sdramTiming.RPDelay              = timingConfig.rpDelay;
    sdramTiming.RCDDelay             = timingConfig.rcdDelay;

    HAL_SDRAM_Init(&sdram, &sdramTiming);
}

SDRAM::Status SDRAMf4xx::EnableWriteProtection() {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_WriteProtection_Enable(this->sdramDevice, this->initConfig.sdBank);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::DisableWriteProtection() {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_WriteProtection_Disable(this->sdramDevice, this->initConfig.sdBank);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::SendCommand(SDRAMCommand type, SDRAMBank target, uint16_t refreshNumber, uint16_t modeRegister) {
    FMC_SDRAM_CommandTypeDef halCommand{};

    halCommand.CommandMode            = static_cast<uint32_t>(type);
    halCommand.CommandTarget          = static_cast<uint32_t>(target);
    halCommand.AutoRefreshNumber      = refreshNumber;
    halCommand.ModeRegisterDefinition = modeRegister;

    HAL_StatusTypeDef halStatus = FMC_SDRAM_SendCommand(this->sdramDevice, &halCommand, 0xFFFF);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::ProgramRefreshRate(uint32_t refreshRate) {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_ProgramRefreshRate(this->sdramDevice, refreshRate);
    
    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::SetAutoRefreshNumber(uint32_t autoRefreshNumber) {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_SetAutoRefreshNumber(this->sdramDevice, autoRefreshNumber);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::SDRAMState SDRAMf4xx::GetModeStatus() {
    switch(FMC_SDRAM_GetModeStatus(this->sdramDevice, this->initConfig.sdBank)) {
    case FMC_SDRAM_SELF_REFRESH_MODE:
        return SDRAMState::SELF_REFRESH_MODE;
        break;
    case FMC_SDRAM_POWER_DOWN_MODE:
        return SDRAMState::POWER_DOWN_MODE;
        break;
    default:
    case FMC_SDRAM_NORMAL_MODE:
        return SDRAMState::NORMAL_MODE;
        break;
    }
}

SDRAM::SDRAMInitConfig SDRAMf4xx::defaultSdramInitConfig() {
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
    return memoryAddress; // This is a protected variable from the SDRAM class definition
}

void SDRAMf4xx::InitHardware(Pin* pins) {
    __HAL_RCC_FMC_CLK_ENABLE();

    // TODO: FIX WITH GPIO BULK INIT (ON DIFFERENT BRANCH)
}

} // namespace core::io