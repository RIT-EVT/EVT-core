#ifndef _EVT_SPIf302x8_
#define _EVT_SPIf302x8_

#include <stdint.h>

#include <EVT/io/SPI.hpp>
#include <HALf3/stm32f3xx.h>

#define SPI_MAX_BAUD 4000000

namespace EVT::core::IO {
    class SPIf302x8: public SPI{
    public:
        SPIf302x8(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin);
        SPIf302x8(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin);
        void configureSPI(uint32_t baudRate, uint8_t mode, uint8_t order);
        void startTransmition(uint8_t device);
        void endTransmition(uint8_t device);
        void write(uint8_t byte);
        uint8_t read();
    private:
        SPI_HandleTypeDef halSPI;
    };
}

#endif