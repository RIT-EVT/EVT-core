#ifndef _EVT_SPIf3xx_
#define _EVT_SPIf3xx_

#include <cstdint>

#include <EVT/io/SPI.hpp>
#include <HALf3/stm32f3xx.h>

#define SPI_MAX_BAUD 4000000

namespace EVT::core::IO {
class SPIf3xx : public SPI {
public:
    SPIf3xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin);
    SPIf3xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin);
    void configureSPI(uint32_t baudRate, uint8_t mode, uint8_t order) override;

    bool startTransmission(uint8_t device) override;
    bool endTransmission(uint8_t device) override;

    void write(uint8_t byte) override;
    uint8_t read() override;

    void write(uint8_t* bytes, uint8_t length) override;
    void read(uint8_t* bytes, uint8_t length) override;

private:
    void togglePin(GPIO* pin);

    static uint8_t getMOSIPortID(Pin mosiPin);
    static uint8_t getMISOPortID(Pin misoPin);
    static uint8_t getSCKPortID(Pin sckPin);

    SPI_HandleTypeDef halSPI;
};
}// namespace EVT::core::IO

#endif