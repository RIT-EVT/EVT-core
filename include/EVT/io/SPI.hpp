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

        /**
         * begin a device transmission. Call before each set of read and write interactions.
         * @param device the device number in the CSPins array
         * @return true if valid device, false if device not in CSPins
         */
        virtual bool startTransmition(uint8_t device) = 0;

        /**
        * toggle the state of the chip select pin of a device back at the end of a transmission. Call when finished
        * reading or writing a data packet.
        * @param device the device index in the CSPins
        * @return true if valid device, false if device not in CSPins
        */
        virtual bool endTransmition(uint8_t device) = 0;

        /**
         * Writes a single byte out to the SPI device. Call startTransmition() first to initiate device communication.
         * @param byte the byte to write
         */
        virtual void write(uint8_t byte) = 0;

        /**
         * reads a single byte from a SPI device. Call startTransmition() first to initiate device communication.
         * @return the byte read
         */
        virtual uint8_t read() = 0;

//        void write(uint8_t device, uint8_t byte);
//
//        uint8_t read(uint8_t device);
        /**
         * writes an array of bytes to the SPI device. Call startTransmition() first to initiate device communication.
         * @param bytes an array of bytes of length n to write to SPI device
         * @param length the length of the array
         */
        void write(uint8_t *bytes, uint8_t length);

        /**
        * reads an array of bytes from a SPI device. Call startTransmition() first to initiate device communication.
        * @param bytes an array of length n to receive the bytes from an SPI device
        * @param length the number of bytes to recive
        */
        void read(uint8_t *bytes, uint8_t length);

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

    protected:
        uint8_t CSPinsLength;
        GPIO *CSPins;
    };

}  // namespace EVT::core::IO

#endif
