#include <core/io/FMC.hpp>

namespace core::io {

FMC::FMC(void* memoryAddress, const FMCPinConfig& pinConfig) : fmcPinConfig(pinConfig), memoryAddress(memoryAddress) {}

void* FMC::getStartingAddress() const {
    return this->memoryAddress;
}

} // namespace core::io