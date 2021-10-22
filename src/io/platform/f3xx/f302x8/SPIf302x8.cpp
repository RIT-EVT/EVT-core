//
// Created by matth on 10/2/2021.
//

#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f3xx/f302x8/SPIf302x8.hpp>

namespace EVT::core::IO {

    static uint8_t getMOSIPortID(Pin mosiPin) {
        switch (mosiPin) {
            case Pin::PA_11:
            case Pin::PB_15:
                return 2;
            case Pin::PB_5:
            case Pin::PC_12:
                return 3;
            default:
                return 0;
        }
    }

    static uint8_t getMISOPortID(Pin misoPin) {
        switch (misoPin) {
            case Pin::PA_10:
            case Pin::PB_14:
                return 2;
            case Pin::PB_4:
            case Pin::PC_11:
                return 3;
            default:
                return 0;
        }
    }

    static uint8_t getSCKPortID(Pin sckPin) {
        switch (sckPin) {
            case Pin::PA_10:
            case Pin::PF_1:
                return 2;
            case Pin::PB_3:
            case Pin::PC_10:
                return 3;
            default:
                return 0;
        }
    }

    /**
     * 
     * @param CSPins 
     * @param pinLength 
     * @param sckPin 
     * @param mosiPin 
     * @param misoPin 
     */
    SPIf302x8::SPIf302x8(GPIO **CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) :
                        SPI(CSPins, pinLength, sckPin, mosiPin, misoPin) {

        uint8_t mosiPort = getMOSIPortID(mosiPin);
        uint8_t misoPort = getMISOPortID(misoPin);
        uint8_t sckPort = getSCKPortID(sckPin);
        GPIO_InitTypeDef GPIOInit = {0};
        if (mosiPort == misoPort && misoPort == sckPort) {
            switch (mosiPort){
                case 2:
                    halSPI.Instance == SPI2;
                    if (!__HAL_RCC_SPI2_IS_CLK_ENABLED()) {
                        __HAL_RCC_SPI2_CLK_ENABLE();
                    }
                    GPIOInit.Alternate = GPIO_AF5_SPI2;
                    break;
                case 3:
                    halSPI.Instance == SPI3;
                    if (!__HAL_RCC_SPI3_IS_CLK_ENABLED()) {
                        __HAL_RCC_SPI3_CLK_ENABLE();
                    }
                    GPIOInit.Alternate = GPIO_AF6_SPI3;
                    break;
                default:
                    break;
            }

            Pin spiPins[] = {mosiPin, misoPin, sckPin};

            GPIOInit.Mode = GPIO_MODE_AF_PP;
            GPIOInit.Pull = GPIO_NOPULL;
            GPIOInit.Speed = GPIO_SPEED_FREQ_HIGH;

            for (uint8_t i = 0; i < 3; i++) {
                GPIOInit.Pin = static_cast<uint32_t>(1
                                << (static_cast<uint32_t>(spiPins[i]) & 0x0F));
                switch ((static_cast<uint8_t>(spiPins[i]) & 0xF0) >> 4) {
                    case 0x0:
                        __HAL_RCC_GPIOA_CLK_ENABLE();
                        HAL_GPIO_Init(GPIOA, &GPIOInit);
                        break;
                    case 0x1:
                        __HAL_RCC_GPIOB_CLK_ENABLE();
                        HAL_GPIO_Init(GPIOB, &GPIOInit);
                        break;
                    case 0x2:
                        __HAL_RCC_GPIOC_CLK_ENABLE();
                        HAL_GPIO_Init(GPIOC, &GPIOInit);
                        break;
                    case 0x3:
                        __HAL_RCC_GPIOD_CLK_ENABLE();
                        HAL_GPIO_Init(GPIOD, &GPIOInit);
                        break;
                    case 0x5:
                        __HAL_RCC_GPIOF_CLK_ENABLE();
                        HAL_GPIO_Init(GPIOF, &GPIOInit);
                        break;
                    default:
                        break;
                }
            }

            halSPI.Init.Mode = SPI_MODE_MASTER;
            halSPI.Init.Direction = SPI_DIRECTION_2LINES;
            halSPI.Init.DataSize = SPI_DATASIZE_8BIT;
            
            //advanced settings
            halSPI.Init.TIMode = SPI_TIMODE_DISABLE;
            halSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
            halSPI.Init.CRCPolynomial = 7;
            halSPI.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
            halSPI.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
            halSPI.Init.NSS = SPI_NSS_SOFT;
        }
    }



    SPIf302x8::SPIf302x8(GPIO **CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin) :
            SPI(CSPins,pinLength,sckPin, mosiPin) {

    }

    void SPIf302x8::configureSPI(uint32_t baudRate, uint8_t mode, uint8_t order) {
        //set the CPOL and CPHA depending on the SPI mode
        switch(mode){
            case SPIMode0:
                halSPI.Init.CLKPolarity = SPI_POLARITY_LOW;
                halSPI.Init.CLKPhase = SPI_PHASE_1EDGE;
                break;
            case SPIMode1:
                halSPI.Init.CLKPolarity = SPI_POLARITY_LOW;
                halSPI.Init.CLKPhase = SPI_PHASE_2EDGE;
                break;
            case SPIMode2:
                halSPI.Init.CLKPolarity = SPI_POLARITY_HIGH;
                halSPI.Init.CLKPhase = SPI_PHASE_1EDGE;
                break;
            case SPIMode3:
                halSPI.Init.CLKPolarity = SPI_POLARITY_HIGH;
                halSPI.Init.CLKPhase = SPI_PHASE_2EDGE;
                break;
        }

        //configure the clock prescaler to the closest baudrate to the requested
        if (baudRate >= SPI_MAX_BAUD) {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        } else if (baudRate >= SPI_MAX_BAUD/2) {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
        } else if (baudRate >= SPI_MAX_BAUD/4) {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
        } else if (baudRate >= SPI_MAX_BAUD/8) {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        } else if (baudRate >= SPI_MAX_BAUD/16) {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
        } else if (baudRate >= SPI_MAX_BAUD/32) {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
        } else if (baudRate >= SPI_MAX_BAUD/64) {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
        } else {
            halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
        }

        //configure the bit order of the data; MSB or LSB
        if(order){
            halSPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
        }else{
            halSPI.Init.FirstBit = SPI_FIRSTBIT_LSB;
        }

        HAL_SPI_Init(&halSPI);
    }

    void SPIf302x8::startTransmition(uint8_t device) {

    }

    void SPIf302x8::endTransmition(uint8_t device) {

    }

    void SPIf302x8::write(uint8_t byte) {

    }

    uint8_t SPIf302x8::read() {
        return 0;
    }
}

