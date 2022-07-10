#include <EVT/io/SPI.hpp>

namespace EVT::core::IO {
SPI::SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) : CSPinsLength(pinLength),
                                                                                    sckPin(sckPin),
                                                                                    mosiPin(mosiPin),
                                                                                    misoPin(misoPin) {
    if (pinLength >= MAX_PINS) {
        CSPinsLength = MAX_PINS;
    }
    for (int i = 0; i < CSPinsLength; i++) {
        this->CSPins[i] = CSPins[i];
    }
}

SPI::SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin) : CSPinsLength(pinLength), sckPin(sckPin),
                                                                       mosiPin(mosiPin) {
    if (pinLength >= MAX_PINS) {
        CSPinsLength = MAX_PINS;
    }
    for (int i = 0; i < CSPinsLength; i++) {
        this->CSPins[i] = CSPins[i];
    }
}

SPI::SPIStatus SPI::write(uint8_t* bytes, uint8_t length) {
    SPIStatus status;

    for (int i = 0; i < length; i++) {
        status = write(bytes[i]);
        if (status != SPIStatus::OK)
            return status;
    }

    return SPIStatus::OK;
}

SPI::SPIStatus SPI::read(uint8_t* bytes, uint8_t length) {
    SPIStatus status;
    for (int i = 0; i < length; i++) {
        status = read(&bytes[i]);
        if (status != SPIStatus::OK)
            return status;
    }

    return SPIStatus::OK;
}

SPI::SPIStatus SPI::writeReg(uint8_t device, uint8_t reg, uint8_t byte) {
    bool transmitSuccess = false;
    SPIStatus status;

    if (startTransmission(device)) {
        status = write(reg);
        if (status != SPIStatus::OK)
            return status;

        status = write(byte);
        if (status != SPIStatus::OK)
            return status;

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

SPI::SPIStatus SPI::readReg(uint8_t device, uint8_t reg, uint8_t* out) {
    bool transmitSuccess = false;
    SPIStatus status;

    if (startTransmission(device)) {
        status = write(reg);
        if (status != SPIStatus::OK)
            return status;

        status = read(out);
        if (status != SPIStatus::OK)
            return status;

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

SPI::SPIStatus SPI::writeReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length) {
    bool transmitSuccess = false;
    SPIStatus status;

    if (startTransmission(device)) {
        status = write(reg);
        if (status != SPIStatus::OK)
            return status;

        status = write(bytes, length);
        if (status != SPIStatus::OK)
            return status;

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

SPI::SPIStatus SPI::readReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length) {
    bool transmitSuccess = false;
    SPIStatus status;

    if (startTransmission(device)) {
        status = write(reg);
        if (status != SPIStatus::OK)
            return status;

        status = read(bytes, length);
        if (status != SPIStatus::OK)
            return status;

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

}// namespace EVT::core::IO
