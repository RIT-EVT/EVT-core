#ifndef _EVT_SPI_
#define _EVT_SPI_

#include <stdint.h>
#include "GPIO.hpp"

#define SPI_MSB_FIRST 1
#define SPI_LSB_FIRST 0

#define SPIMode0 0x01
#define SPIMode1 0x02
#define SPIMode2 0x04
#define SPIMode3 0x08

namespace EVT::core::IO {
    // Forward declarations:
    // The different pins are hardware specific. Forward declaration to allow
    // at compilation time the decision of which pins should be used.
    enum class Pin;

    class SPI {
    public:
        /**
         * Make an instance of an I2C interface that will use the given pins
         * for clock and data lines.
         *
         * @param sckPin[in] The SPI clock pin
         * @param mosiPin[in] The mosi pin (data out)
         * @param misoPin[in] The miso pin (data in)[optional]
         */
        SPI(GPIO **CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin);

        SPI(GPIO **CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin);

        void write(uint8_t device, uint8_t byte);

        uint8_t read(uint8_t device);

        void write(uint8_t device, uint8_t *bytes, uint8_t length);

        void read(uint8_t device, uint8_t *bytes, uint8_t length);

        void writeReg(uint8_t device, uint8_t reg, uint8_t byte);

        uint8_t readReg(uint8_t device, uint8_t reg);

        virtual void configureSPI(uint32_t baudRate, uint8_t mode, uint8_t order) = 0;

    private:
        /** The SPI clock line */
        Pin sckPin;
        /** The MOSI data line*/
        Pin mosiPin;
        /** The MISO data line */
        Pin misoPin;

        virtual bool startTransmition(uint8_t device) = 0;

        virtual bool endTransmition(uint8_t device) = 0;

        virtual void write(uint8_t byte) = 0;

        virtual uint8_t read() = 0;

    protected:
        uint8_t CSPinsLength;
        GPIO *CSPins;
    };

}  // namespace EVT::core::IO

#endif
