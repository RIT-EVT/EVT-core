#include "core/io/FMC.hpp"

namespace core::io {

FMC::FMC(uint32_t sdramMemoryAddress) {
    this->sdramMemoryAddress = sdramMemoryAddress;
}

} // namespace core::io