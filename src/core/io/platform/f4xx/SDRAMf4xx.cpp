#include <core/io/platform/f4xx/GPIOf4xx.hpp>
#include <core/io/platform/f4xx/SDRAMf4xx.hpp>
#include <core/utils/log.hpp>

namespace core::io {

SDRAMf4xx::SDRAMf4xx(SDRAMPinGroup& pins, const SDRAMInitConfig& sdramInitConfig,
                     const SDRAMTimingConfig& sdramTimingConfig)
    : SDRAM((sdramInitConfig.sdBank == FMC_SDRAM_BANK1)
            ? reinterpret_cast<uint32_t*>(SDRAM_BANK1) : reinterpret_cast<uint32_t*>(SDRAM_BANK2),
            pins, sdramInitConfig, sdramTimingConfig),
      sdramDevice(FMC_SDRAM_DEVICE), sdram(), sdramTiming() {

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
    sdram.Init.CASLatency         = sdramInitConfig.casLatency;
    sdram.Init.SDClockPeriod      = sdramInitConfig.sdClockPeriod;

    sdramTiming.LoadToActiveDelay    = sdramTimingConfig.loadToActiveDelay;
    sdramTiming.ExitSelfRefreshDelay = sdramTimingConfig.exitSelfRefreshDelay;
    sdramTiming.SelfRefreshTime      = sdramTimingConfig.selfRefreshTime;
    sdramTiming.RowCycleDelay        = sdramTimingConfig.rowCycleDelay;
    sdramTiming.WriteRecoveryTime    = sdramTimingConfig.writeRecoveryTime;
    sdramTiming.RPDelay              = sdramTimingConfig.rpDelay;
    sdramTiming.RCDDelay             = sdramTimingConfig.rcdDelay;

    InitHardware(pins);
    HAL_StatusTypeDef status = HAL_SDRAM_Init(&sdram, &sdramTiming);
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "%d status \r\n", status);
}

SDRAM::Status SDRAMf4xx::EnableWriteProtection() {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_WriteProtection_Enable(this->sdramDevice, this->initConfig.sdBank);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::DisableWriteProtection() {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_WriteProtection_Disable(this->sdramDevice, this->initConfig.sdBank);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::SendCommand(SDRAMCommand type, SDRAMCommandTarget target, uint16_t refreshNumber,
                                     uint16_t modeRegister) {
    FMC_SDRAM_CommandTypeDef halCommand{};

    halCommand.CommandMode            = static_cast<uint32_t>(type);
    halCommand.CommandTarget          = static_cast<uint32_t>(target);
    halCommand.AutoRefreshNumber      = refreshNumber;
    halCommand.ModeRegisterDefinition = modeRegister;

    HAL_StatusTypeDef halStatus = FMC_SDRAM_SendCommand(this->sdramDevice, &halCommand, 0xFFFF);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::ProgramRefreshRate(uint32_t rowCount, uint32_t refreshTime) {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_ProgramRefreshRate(this->sdramDevice, (((refreshTime * 1000) / rowCount) * (getSdramClockFrequency() / 1000000)) - 20);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::Status SDRAMf4xx::SetAutoRefreshNumber(uint32_t autoRefreshNumber) {
    HAL_StatusTypeDef halStatus = FMC_SDRAM_SetAutoRefreshNumber(this->sdramDevice, autoRefreshNumber);

    return SDRAM::HALStatusToSDRAMStatus(halStatus);
}

SDRAM::SDRAMState SDRAMf4xx::GetModeStatus() {
    switch (FMC_SDRAM_GetModeStatus(this->sdramDevice, this->initConfig.sdBank)) {
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

void* SDRAMf4xx::getSDRAMMemoryAddress() const {
    return memoryAddress; // This is a protected variable from the SDRAM class definition
}

void SDRAMf4xx::InitHardware(SDRAMPinGroup& pins) {
    __HAL_RCC_FMC_CLK_ENABLE();
    GPIOf4xx::gpioInit(pins.pins, pins.numPins, GPIO_MODE_AF_PP, GPIO_NOPULL,
                           GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);
}

} // namespace core::io