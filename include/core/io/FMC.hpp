#ifndef EVT_FMC_HPP
#define EVT_FMC_HPP

#ifdef STM32F3xx
    #error // Controlling SDRAM through an FMC is not supported on F3xx chips
#endif

namespace core::io {

/**
 * The FMC is a Flexible Memory Controller, F3xx board do not support SDRAM so there are no common functions.
 */
class FMC {};

} // namespace core::io

#endif // EVT_FMC_HPP
