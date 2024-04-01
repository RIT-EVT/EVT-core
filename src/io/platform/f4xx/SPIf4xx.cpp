#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f4xx/GPIOf4xx.hpp>
#include <EVT/io/platform/f4xx/SPIf4xx.hpp>
#include <EVT/utils/log.hpp>

namespace log = EVT::core::log;

namespace EVT::core::IO {

void SPIf4xx::togglePin(GPIO* pin) {
    switch (pin->readPin()) {
    case GPIO::State::HIGH:
        pin->writePin(GPIO::State::LOW);
        break;
    case GPIO::State::LOW:
        pin->writePin(GPIO::State::HIGH);
        break;
    }
}

uint8_t SPIf4xx::getMOSIPortID(Pin mosiPin) {
#ifdef STM32F446xx
    switch (mosiPin) {
    case Pin::PC_1:
    case Pin::PC_3:
    case Pin::PB_15:
        return 2;
    case Pin::PA_7:
    case Pin::PB_0:
    case Pin::PB_2:
    case Pin::PB_5:
    case Pin::PC_12:
        return 3;
    default:
        return 0;
    }
#endif
}

uint8_t SPIf4xx::getMISOPortID(Pin misoPin) {
#ifdef STM32F446xx
    switch (misoPin) {
    case Pin::PC_2:
    case Pin::PB_14:
        return 2;
    case Pin::PB_4:
    case Pin::PC_11:
        return 3;
    default:
        return 0;
    }
#endif
}

uint8_t SPIf4xx::getSCKPortID(Pin sckPin) {
#ifdef STM32F446xx
    switch (sckPin) {
    case Pin::PB_10:
    case Pin::PB_13:
    case Pin::PC_7:
    case Pin::PA_9:
        return 2;
    case Pin::PB_3:
    case Pin::PC_10:
        return 3;
    default:
        return 0;
    }
#endif
}

SPIf4xx::SPIf4xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) : SPI(CSPins, pinLength, sckPin, mosiPin, misoPin) {
    uint8_t mosiPort = getMOSIPortID(mosiPin);
    uint8_t misoPort = getMISOPortID(misoPin);
    uint8_t sckPort = getSCKPortID(sckPin);
    GPIO_InitTypeDef GPIOInit = {0};
    uint8_t altId = 0x00U;

    if (mosiPort == misoPort && misoPort == sckPort) {
        switch (mosiPort) {
#ifdef STM32F446xx
        case 2:
            halSPI.Instance = SPI2;
            if (!__HAL_RCC_SPI2_IS_CLK_ENABLED()) {
                __HAL_RCC_SPI2_CLK_ENABLE();
            }
            altId = GPIO_AF5_SPI2;
            break;
        case 3:
            halSPI.Instance = SPI3;
            if (!__HAL_RCC_SPI3_IS_CLK_ENABLED()) {
                __HAL_RCC_SPI3_CLK_ENABLE();
            }
            altId = GPIO_AF5_SPI3;
            break;
#endif
        default:
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid SPI mosiPort");
            break;
        }

        // gpioStateInit only supports initializing up to 2 pins, so this must be done
        // init mosiPin and misoPin
        Pin spiPins[] = {mosiPin, misoPin};
        uint8_t numOfPins = 2;

        GPIOf4xx::gpioStateInit(&GPIOInit, spiPins, numOfPins, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, altId);
        // init sckPin
        spiPins[0] = sckPin;
        numOfPins = 1;
        GPIOf4xx::gpioStateInit(&GPIOInit, spiPins, numOfPins, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, altId);

        halSPI.Init.Mode = SPI_MODE_MASTER;
        halSPI.Init.Direction = SPI_DIRECTION_2LINES;
        halSPI.Init.DataSize = SPI_DATASIZE_8BIT;

        // advanced settings
        halSPI.Init.TIMode = SPI_TIMODE_DISABLE;
        halSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        halSPI.Init.CRCPolynomial = 7;
        halSPI.Init.NSS = SPI_NSS_SOFT;
    }
}

SPIf4xx::SPIf4xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin) : SPI(CSPins, pinLength, sckPin, mosiPin) {
    uint8_t mosiPort = getMOSIPortID(mosiPin);
    uint8_t sckPort = getSCKPortID(sckPin);
    GPIO_InitTypeDef GPIOInit = {0};
    uint8_t altId = 0x00U;

    if (mosiPort == sckPort) {
        switch (mosiPort) {
#ifdef STM32F446xx
        case 2:
            halSPI.Instance = SPI2;
            if (!__HAL_RCC_SPI2_IS_CLK_ENABLED()) {
                __HAL_RCC_SPI2_CLK_ENABLE();
            }
            altId = GPIO_AF5_SPI2;
            break;
        case 3:
            halSPI.Instance = SPI3;
            if (!__HAL_RCC_SPI3_IS_CLK_ENABLED()) {
                __HAL_RCC_SPI3_CLK_ENABLE();
            }
            altId = GPIO_AF5_SPI3;
            break;
#endif
        default:
            break;
        }

        Pin spiPins[] = {mosiPin, sckPin};
        uint8_t numOfPins = 2;

        GPIOf4xx::gpioStateInit(&GPIOInit, spiPins, numOfPins, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, altId);

        halSPI.Init.Mode = SPI_MODE_MASTER;
        halSPI.Init.Direction = SPI_DIRECTION_1LINE;
        halSPI.Init.DataSize = SPI_DATASIZE_8BIT;

        // advanced settings
        halSPI.Init.TIMode = SPI_TIMODE_DISABLE;
        halSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        halSPI.Init.CRCPolynomial = 7;
        halSPI.Init.NSS = SPI_NSS_SOFT;
    }
}

void SPIf4xx::configureSPI(uint32_t baudRate, uint8_t mode, uint8_t firstBitMSB) {
    // set the CPOL and CPHA depending on the SPI mode
    switch (mode) {
    case SPI_MODE0:
        halSPI.Init.CLKPolarity = SPI_POLARITY_LOW;
        halSPI.Init.CLKPhase = SPI_PHASE_1EDGE;
        break;
    case SPI_MODE1:
        halSPI.Init.CLKPolarity = SPI_POLARITY_LOW;
        halSPI.Init.CLKPhase = SPI_PHASE_2EDGE;
        break;
    case SPI_MODE2:
        halSPI.Init.CLKPolarity = SPI_POLARITY_HIGH;
        halSPI.Init.CLKPhase = SPI_PHASE_1EDGE;
        break;
    case SPI_MODE3:
        halSPI.Init.CLKPolarity = SPI_POLARITY_HIGH;
        halSPI.Init.CLKPhase = SPI_PHASE_2EDGE;
        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid SPI Mode");
        break;
    }

    // configure the clock prescaler to the closest baudrate to the requested
    if (baudRate >= SPI_MAX_BAUD) {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    } else if (baudRate >= SPI_MAX_BAUD / 2) {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    } else if (baudRate >= SPI_MAX_BAUD / 4) {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    } else if (baudRate >= SPI_MAX_BAUD / 8) {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    } else if (baudRate >= SPI_MAX_BAUD / 16) {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    } else if (baudRate >= SPI_MAX_BAUD / 32) {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    } else if (baudRate >= SPI_MAX_BAUD / 64) {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    } else {
        halSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    }

    // configure the bit order of the data; MSB or LSB
    if (firstBitMSB) {
        halSPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
    } else {
        halSPI.Init.FirstBit = SPI_FIRSTBIT_LSB;
    }

    HAL_SPI_Init(&halSPI);
}

bool SPIf4xx::startTransmission(uint8_t device) {
    if (device < CSPinsLength) {
        togglePin(CSPins[device]);
        return true;
    }
    return false;
}

bool SPIf4xx::endTransmission(uint8_t device) {
    if (device < CSPinsLength) {
        togglePin(CSPins[device]);
        return true;
    }
    return false;
}

SPI::SPIStatus SPIf4xx::write(uint8_t byte) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Transmit(&halSPI, &byte, 1, EVT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

SPI::SPIStatus SPIf4xx::read(uint8_t* out) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Receive(&halSPI, out, 1, EVT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

SPI::SPIStatus SPIf4xx::write(uint8_t* bytes, uint8_t length) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Transmit(&halSPI, bytes, length, EVT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

SPI::SPIStatus SPIf4xx::read(uint8_t* bytes, uint8_t length) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Receive(&halSPI, bytes, length, EVT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

/**
 * Converts HAL Status into EVT Core's own SPIStatus enum
 *
 * @param[in] halStatus HAL-defined status enum value
 * @return SPIStatus converted from halStatus
 */
SPI::SPIStatus SPIf4xx::halToSPIStatus(HAL_StatusTypeDef halStatus) {
    switch (halStatus) {
    case HAL_OK: return SPIStatus::OK;
    case HAL_ERROR: return SPIStatus::ERROR;
    case HAL_BUSY: return SPIStatus::BUSY;
    case HAL_TIMEOUT: return SPIStatus::TIMEOUT;
    }
}

}// namespace EVT::core::IO
