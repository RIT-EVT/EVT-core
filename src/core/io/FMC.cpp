#include "core/io/FMC.hpp"

namespace core::io {

FMC::FMC(void* sdramMemoryAddress, FMCPinConfig pinConfig, SdramInitConfig sdramInitConfig,
         SdramTimingConfig sdramTimingConfig)
    : sdramMemoryAddress(sdramMemoryAddress), fmcPinConfig(pinConfig), sdramInitConfig(sdramInitConfig),
      sdramTimingConfig(sdramTimingConfig) {}

} // namespace core::io