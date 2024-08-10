#include <core/io/SPI.hpp>

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b

#define SPI_CHECK(functionCall)                        \
    SPIStatus CONCAT(status, __LINE__) = functionCall; \
    if (CONCAT(status, __LINE__) != SPIStatus::OK)     \
    return CONCAT(status, __LINE__)

namespace EVT::core::IO {

SPI::SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) : sckPin(sckPin),
                                                                                    mosiPin(mosiPin),
                                                                                    misoPin(misoPin),
                                                                                    CSPinsLength(pinLength) {
    if (pinLength >= MAX_PINS) {
        CSPinsLength = MAX_PINS;
    }
    for (int i = 0; i < CSPinsLength; i++) {
        this->CSPins[i] = CSPins[i];
    }
}

SPI::SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin) : sckPin(sckPin),
                                                                       mosiPin(mosiPin),
                                                                       // There is no MISO pin
                                                                       misoPin(static_cast<Pin>(0)),
                                                                       CSPinsLength(pinLength) {
    if (pinLength >= MAX_PINS) {
        CSPinsLength = MAX_PINS;
    }
    for (int i = 0; i < CSPinsLength; i++) {
        this->CSPins[i] = CSPins[i];
    }
}

SPI::SPIStatus SPI::write(uint8_t* bytes, uint8_t length) {

    for (int i = 0; i < length; i++) {
        SPI_CHECK(write(bytes[i]));
    }

    return SPIStatus::OK;
}

SPI::SPIStatus SPI::read(uint8_t* bytes, uint8_t length) {
    for (int i = 0; i < length; i++) {
        SPI_CHECK(read(&bytes[i]));
    }

    return SPIStatus::OK;
}

SPI::SPIStatus SPI::writeReg(uint8_t device, uint8_t reg, uint8_t byte) {
    bool transmitSuccess = false;

    if (startTransmission(device)) {
        SPI_CHECK(write(reg));

        SPI_CHECK(write(byte));

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

SPI::SPIStatus SPI::readReg(uint8_t device, uint8_t reg, uint8_t* out) {
    bool transmitSuccess = false;

    if (startTransmission(device)) {
        SPI_CHECK(write(reg));

        SPI_CHECK(read(out));

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

SPI::SPIStatus SPI::writeReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length) {
    bool transmitSuccess = false;

    if (startTransmission(device)) {
        SPI_CHECK(write(reg));
        SPI_CHECK(write(bytes, length));

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

SPI::SPIStatus SPI::readReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length) {
    bool transmitSuccess = false;

    if (startTransmission(device)) {
        SPI_CHECK(write(reg));

        SPI_CHECK(read(bytes, length));

        transmitSuccess = endTransmission(device);
    }

    return transmitSuccess ? SPIStatus::OK : SPIStatus::ERROR;
}

}// namespace EVT::core::IO
