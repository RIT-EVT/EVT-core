#include "core/io/FMC.hpp"

namespace core::io {

FMC::FMC(FMCPinConfig pinConfig, SdramInitConfig sdramInitConfig, SdramTimingConfig sdramTimingConfig) :
    fmcPinConfig(pinConfig),
    sdramInitConfig(sdramInitConfig),
    sdramTimingConfig(sdramTimingConfig) {}

} // namespace core::io