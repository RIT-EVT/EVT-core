#include <core/io/FMC.hpp>

namespace core::io {

FMC::FMC(void* memoryAddress, FMCPinConfig pinConfig)
    : memoryAddress(memoryAddress)
    , fmcPinConfig(pinConfig) {}

} // namespace core::io