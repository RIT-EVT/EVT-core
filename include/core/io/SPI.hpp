    #ifndef _EVT_SPI_
#define _EVT_SPI_

#include <cstdint>

#include <core/io/GPIO.hpp>

#ifndef EVT_SPI_TIMEOUT
    #define EVT_SPI_TIMEOUT 100
#endif

#define SPI_MSB_FIRST true
#define SPI_LSB_FIRST false

#define SPI_SPEED_4MHZ   4000000
#define SPI_SPEED_2MHZ   2000000
#define SPI_SPEED_1MHZ   1000000
#define SPI_SPEED_500KHZ 500000
#define SPI_SPEED_250KHZ 250000
#define SPI_SPEED_125KHZ 125000
#define SPI_SPEED_62KHZ  62500
#define SPI_SPEED_31KHZ  31250

namespace core::io {

// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

class SPI {
public:
    /**
     * Represents the current state of using the SPI interface.
     */
    enum class SPIStatus {
        OK      = 0,
        ERROR   = 1,
        BUSY    = 2,
        TIMEOUT = 3,
    };

    /**
     * Enum of SPI mode's that can be used.
     */
    enum class SPIMode {
        SPI_MODE0 = 0x01,
        SPI_MODE1 = 0x02,
        SPI_MODE2 = 0x04,
        SPI_MODE3 = 0x08,
    };

    /**
     * Constructs an SPI instance in full duplex mode to send and receive data.
     *
     * @param CSPins an array of chip select pins for selecting which device to communicate with.
     * @param pinLength the number of pins in the chip select array
     * @param sckPin the pin for the clk line
     * @param mosiPin the mosi pin for sending data
     * @param misoPin the miso pin for receiving data
     */
    SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin);

    /**
     * Constructs an SPI instance in half duplex mode to only send data.
     *
     * @param CSPins an array of chip select pins for selecting which device to communicate with.
     * @param pinLength the number of pins in the chip select array
     * @param sckPin the pin for the clk line
     * @param mosiPin the mosi pin for sending data
     */
    SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin);

    /**
     * Begin a device transmission. Call before each set of read and write interactions.
     *
     * @param device the device number in the CSPins array
     * @return true if valid device, false if device not in CSPins
     */
    virtual bool startTransmission(uint8_t device) = 0;

    /**
     * Toggle the state of the chip select pin of a device back at the end of a transmission. Call when finished
     * reading or writing a data packet.
     *
     * @param device the device index in the CSPins
     * @return true if valid device, false if device not in CSPins
     */
    virtual bool endTransmission(uint8_t device) = 0;

    /**
     * Writes a single byte out to the SPI device. Call startTransmission() first to initiate device communication.
     *
     * @param byte the byte to write
     * @return the status after calling the function
     */
    virtual SPI::SPIStatus write(uint8_t byte) = 0;

    /**
     * Reads a single byte from a SPI device. Call startTransmission() first to initiate device communication.
     *
     * @param out the byte read
     * @return the status after calling the function
     */
    virtual SPI::SPIStatus read(uint8_t* out) = 0;

    /**
     * Writes an array of bytes to the SPI device. Call startTransmission() first to initiate device communication.
     *
     * @param bytes an array of bytes of length n to write to SPI device
     * @param length the length of the array
     * @return the status after calling the function
     */
    virtual SPI::SPIStatus write(uint8_t* bytes, uint8_t length);

    /**
     * Reads an array of bytes from a SPI device.
     *
     * @param device the device to write to in CSPins
     * @param bytes an array of length n to receive the bytes from an SPI device
     * @param length the number of bytes to receive
     * @return SPIStatus of the HAL function call
     */
    virtual SPI::SPIStatus read(uint8_t* bytes, uint8_t length);

    /**
     * Writes a byte of data to a register of a device.
     *
     * @param device the device index in the CSPins
     * @param reg the register address to write to
     * @param byte the byte of data to write
     * @return the status after calling the function
     */
    SPI::SPIStatus writeReg(uint8_t device, uint8_t reg, uint8_t byte);

    /**
     * Reads a byte of data from a register from a device.
     *
     * @param device the device index in the CSPins
     * @param reg the register address to read from
     * @param out the byte of data from the device
     * @return the status after calling the function
     */
    SPI::SPIStatus readReg(uint8_t device, uint8_t reg, uint8_t* out);

    /**
     * Writes a series of bytes to a device's registers starting at a specific one.
     * (Device must support a multi-byte write)
     *
     * @param device the device index in the CSPins
     * @param reg the register address to start the write at
     * @param bytes an array of bytes of length n to write to SPI device
     * @param length the length of the array
     * @return the status after calling the function
     */
    SPI::SPIStatus writeReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length);

    /**
     * Reads a series of bytes from a device's registers starting at a specific one.
     *
     * @param device the device index in the CSPins
     * @param reg the register address to start the read from
     * @param bytes an array of bytes of length n to store the byte from an SPI device
     * @param length the length of the array
     * @return the status after calling the function
     */
    SPI::SPIStatus readReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length);

    /**
     * Configures the SPI transmit mode.
     *
     * @param baudRate the baudrate to transmit at (4MHz to 31.25KHz)
     * @param mode The SPIMode to use when sending (0-3)
     * @param order MSB first (true) or LSB first (false)
     */
    virtual void configureSPI(uint32_t baudRate, SPIMode mode, bool order) = 0;

private:
    /** The SPI clock line */
    Pin sckPin;
    /** The MOSI data line*/
    Pin mosiPin;
    /** The MISO data line */
    Pin misoPin;

protected:
    static constexpr uint8_t MAX_PINS = 5;
    uint8_t CSPinsLength;
    GPIO* CSPins[MAX_PINS] = {};
};

} // namespace core::io

#endif
