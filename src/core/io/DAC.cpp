#include <core/io/DAC.hpp>

namespace core::io {

void DACBase::setValue(uint16_t value) {
    writeRaw(value > MAX_RAW ? MAX_RAW : value);
}

uint16_t DACBase::getValue() const {
    return readRaw();
}

void DACBase::setVoltage(float voltage) {
    if (voltage <= 0.0f) {
        writeRaw(0);
    } else if (voltage >= vref) {
        writeRaw(MAX_RAW);
    } else {
        // Round to the nearest step rather than truncating
        writeRaw(static_cast<uint16_t>(voltage / vref * MAX_RAW + 0.5f));
    }
}

float DACBase::getVoltage() const {
    return static_cast<float>(readRaw()) * vref / MAX_RAW;
}

void DACBase::setVref(float vref) {
    this->vref = vref;
}

float DACBase::getVref() const {
    return vref;
}

bool DACBase::isBufferRunning() const {
    return bufferRunning;
}

void DACBase::setCallback(DACCallback callback, void* priv) {
    this->callback = callback;
    callbackPriv   = priv;
}

void DACBase::notify(DACEvent event) {
    if (callback) {
        callback(event, callbackPriv);
    }
}

} // namespace core::io
