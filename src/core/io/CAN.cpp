#include <stdint.h>

#include <core/io/CAN.hpp>

namespace core::io {

CAN::CAN(Pin txPin, Pin rxPin, bool loopbackEnabled) : txPin(txPin), rxPin(rxPin), loopbackEnabled(loopbackEnabled) {
    this->handler = nullptr;
    this->priv    = nullptr;
}

void CAN::addIRQHandler(void (*handler)(CANMessage&, void*), void* priv) {
    this->handler = handler;
    this->priv    = priv;
}

} // namespace core::io
