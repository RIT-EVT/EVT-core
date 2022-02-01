#ifndef _EVT_SPI_
#define _EVT_SPI_

#include "GPIO.hpp"
#include <stdint.h>

#define SPI_MSB_FIRST 1
#define SPI_LSB_FIRST 0

#define SPI_MODE0 0x01
#define SPI_MODE1 0x02
#define SPI_MODE2 0x04
#define SPI_MODE3 0x08

#define SPI_SPEED_4MHZ 4000000
#define SPI_SPEED_2MHZ 2000000
#define SPI_SPEED_1MHZ 1000000
#define SPI_SPEED_500KHZ 500000
#define SPI_SPEED_250KHZ 250000
#define SPI_SPEED_125KHZ 125000
#define SPI_SPEED_62KHZ 62500
#define SPI_SPEED_31KHZ 31250

namespace EVT::core::IO {
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

class SPI {
public:
    /**
     * Constructs an SPI instance in full duplex mode to send and receive data
     * @param CSPins an array of chip select pins for selecting which device to communicate with.
     * @param pinLength the number of pins in the chip select array
     * @param sckPin the pin for the clk line
     * @param mosiPin the mosi pin for sending data
     * @param misoPin the miso pin for receiving data
     */
    SPI(GPIO** CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin);

    /**
     * Constructs an SPI instance in half duplex mode to only send data
     * @param CSPins an array of chip select pins for selecting which device to communicate with.
     * @param pinLength the number of pins in the chip select array
     * @param sckPin the pin for the clk line
     * @param mosiPin the mosi pin for sending data
     */
    SPI(GPIO** CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin);

    /**
     * begin a device transmission. Call before each set of read and write interactions.
     * @param device the device number in the CSPins array
     * @return true if valid device, false if device not in CSPins
     */
    virtual bool startTransmission(uint8_t device) = 0;

    /**
     * toggle the state of the chip select pin of a device back at the end of a transmission. Call when finished
     * reading or writing a data packet.
     * @param device the device index in the CSPins
     * @return true if valid device, false if device not in CSPins
     */
    virtual bool endTransmission(uint8_t device) = 0;

    /**
     * Writes a single byte out to the SPI device. Call startTransmission() first to initiate device communication.
     * @param byte the byte to write
     */
    virtual void write(uint8_t byte) = 0;

    /**
     * reads a single byte from a SPI device. Call startTransmission() first to initiate device communication.
     * @return the byte read
     */
    virtual uint8_t read() = 0;

    /**
     * writes an array of bytes to the SPI device. Call startTransmission() first to initiate device communication.
     * @param bytes an array of bytes of length n to write to SPI device
     * @param length the length of the array
     */
    void write(uint8_t* bytes, uint8_t length);

    /**
     * reads an array of bytes from a SPI device. Call startTransmission() first to initiate device communication.
     * @param bytes an array of length n to receive the bytes from an SPI device
     * @param length the number of bytes to recive
     */
    void read(uint8_t* bytes, uint8_t length);

    /**
     * writes a byte of data to a register of a device
     * @param device the device index in the CSPins
     * @param reg the register address to write to
     * @param byte the byte of data to write
     */
    void writeReg(uint8_t device, uint8_t reg, uint8_t byte);

    /**
     * reads a byte of data from a register from a device
     * @param device the device index in the CSPins
     * @param reg the register address to read from
     * @return the byte of data from the device
     */
    uint8_t readReg(uint8_t device, uint8_t reg);

    /**
     * Writes a series of bytes to a device's registers starting at a specific one.
     * (Device must support a multi-byte write)
     * @param device the device index in the CSPins
     * @param reg the register address to start the write at
     * @param bytes an array of bytes of length n to write to SPI device
     * @param length the length of the array
     */
    void writeReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length);

    /**
     * reads a series of bytes from a device's registers starting at a specific one.
     * @param device the device index in the CSPins
     * @param reg the register address to start the read from
     * @param bytes an array of bytes of length n to store the byte from an SPI device
     * @param length the length of the array
     */
    void readReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length);

    /**
     * Configures the SPI transmit mode
     * @param baudRate the baudrate to transmit at (4MHz to 31.25KHz)
     * @param mode The SPIMode to use when sending (0-3)
     * @param order MSB first or LSB first
     */
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
    GPIO* CSPins;
};

}// namespace EVT::core::IO

#endif
