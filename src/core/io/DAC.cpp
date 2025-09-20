#include <core/io/DAC.hpp>

namespace core::io {

DACBase::DACBase(Pin pin, DACPeriph dacPeriph) : pin(pin), dacPeriph(dacPeriph) {}

} // namespace core::io
