#ifndef _EVT_SPIf3xx_
#define _EVT_SPIf3xx_

#include <cstdint>

#include <HALf3/stm32f3xx.h>

#include <EVT/io/SPI.hpp>

#define SPI_MAX_BAUD 4000000

namespace EVT::core::IO {

class SPIf3xx : public SPI {
public:
    /**
     * Constructs an SPI instance in full duplex mode to send and receive data.
     *
     * @param CSPins an array of chip select pins for selecting which device to communicate with.
     * @param pinLength the number of pins in the chip select array
     * @param sckPin the pin for the clk line
     * @param mosiPin the mosi pin for sending data
     * @param misoPin the miso pin for receiving data
     */
    SPIf3xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin);

    /**
     * Constructs an SPI instance in half duplex mode to only send data.
     *
     * @param CSPins an array of chip select pins for selecting which device to communicate with.
     * @param pinLength the number of pins in the chip select array
     * @param sckPin the pin for the clk line
     * @param mosiPin the mosi pin for sending data
     */
    SPIf3xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin);

    void configureSPI(uint32_t baudRate, uint8_t mode, uint8_t order) override;

    bool startTransmission(uint8_t device) override;
    bool endTransmission(uint8_t device) override;

    void write(uint8_t byte) override;
    uint8_t read() override;

    void write(uint8_t* bytes, uint8_t length) override;
    void read(uint8_t* bytes, uint8_t length) override;

private:
    /**
     * Toggles a GPIO pin's state
     *
     * @param pin the pin to toggle the state of
     */
    static void togglePin(GPIO* pin);

    /**
     * Gets the corresponding SPI port from the mosi pin
     *
     * @param mosiPin the pin to use for the mosi signal
     * @return the SPI port to use, 0 if invalid
     */
    static uint8_t getMOSIPortID(Pin mosiPin);

    /**
     * Gets the corresponding SPI port from the miso pin
     *
     * @param misoPin the pin to use for the miso signal
     * @return the SPI port to use, 0 if invalid
     */
    static uint8_t getMISOPortID(Pin misoPin);

    /**
     * Gets the corresponding SPI port from the sck pin
     *
     * @param sckPin the pin to use for the sck signal
     * @return the SPI port to use, 0 if invalid
     */
    static uint8_t getSCKPortID(Pin sckPin);

    /** Interface into the HAL */
    SPI_HandleTypeDef halSPI = {};
};

}// namespace EVT::core::IO

#endif
