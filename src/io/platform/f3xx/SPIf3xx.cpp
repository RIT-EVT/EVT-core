#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f3xx/GPIOf3xx.hpp>
#include <EVT/io/platform/f3xx/SPIf3xx.hpp>

namespace EVT::core::IO {
/**
 * Gets the corresponding SPI port from the mosi pin
 *
 * @param mosiPin the pin to use for the mosi signal
 * @return the SPI port to use, 0 if invalid
 */
uint8_t SPIf3xx::getMOSIPortID(Pin mosiPin) {
#ifdef STM32F302x8
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
#endif
#ifdef STM32F334x8
    switch (mosiPin) {
    case Pin::PA_7:
    case Pin::PB_5:
        return 1;
    default:
        return 0;
    }
#endif
}

/**
 * Gets the corresponding SPI port from the miso pin
 *
 * @param misoPin the pin to use for the miso signal
 * @return the SPI port to use, 0 if invalid
 */
uint8_t SPIf3xx::getMISOPortID(Pin misoPin) {
#ifdef STM32F302x8
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
#endif
#ifdef STM32F334x8
    switch (misoPin) {
    case Pin::PA_6:
    case Pin::PB_4:
        return 1;
    default:
        return 0;
    }
#endif
}

/**
 * Gets the corresponding SPI port from the sck pin
 *
 * @param sckPin the pin to use for the sck signal
 * @return the SPI port to use, 0 if invalid
 */
uint8_t SPIf3xx::getSCKPortID(Pin sckPin) {
#ifdef STM32F302x8
    switch (sckPin) {
    case Pin::PB_13:
    case Pin::PF_1:
        return 2;
    case Pin::PB_3:
    case Pin::PC_10:
        return 3;
    default:
        return 0;
    }
#endif
#ifdef STM32F334x8
    switch (sckPin) {
    case Pin::PA_5:
    case Pin::PB_3:
        return 1;
    default:
        return 0;
    }
#endif
}

/**
 * Constructs a send and receive SPI object
 *
 * @param CSPins an array of chip select pins for selecting which device to communicate with.
 * @param pinLength the number of pins in the chip select array
 * @param sckPin the pin for the clk line
 * @param mosiPin the mosi pin for sending data
 * @param misoPin the miso pin for receiving data
 */
SPIf3xx::SPIf3xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) : SPI(CSPins, pinLength, sckPin, mosiPin, misoPin) {
    uint8_t mosiPort = getMOSIPortID(mosiPin);
    uint8_t misoPort = getMISOPortID(misoPin);
    uint8_t sckPort = getSCKPortID(sckPin);
    GPIO_InitTypeDef GPIOInit = {0};
    uint8_t altId = 0x00U;

    if (mosiPort == misoPort && misoPort == sckPort) {
        switch (mosiPort) {
#ifdef STM32F334x8
        case 1:
            halSPI.Instance = SPI1;
            if (!__HAL_RCC_SPI1_IS_CLK_ENABLED()) {
                __HAL_RCC_SPI1_CLK_ENABLE();
            }
            altId = GPIO_AF5_SPI1;
            break;
#endif
#ifdef STM32F302x8
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
            altId = GPIO_AF6_SPI3;
            break;
#endif
        default:
            break;
        }

        // gpioStateInit only supports initializing up to 2 pins, so this must be done
        // init mosiPin and misoPin
        Pin spiPins[] = {mosiPin, misoPin};
        uint8_t numOfPins = 2;

        GPIOf3xx::gpioStateInit(&GPIOInit, spiPins, numOfPins, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, altId);
        // init sckPin
        spiPins[0] = sckPin;
        numOfPins = 1;
        GPIOf3xx::gpioStateInit(&GPIOInit, spiPins, numOfPins, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, altId);

        halSPI.Init.Mode = SPI_MODE_MASTER;
        halSPI.Init.Direction = SPI_DIRECTION_2LINES;
        halSPI.Init.DataSize = SPI_DATASIZE_8BIT;

        // advanced settings
        halSPI.Init.TIMode = SPI_TIMODE_DISABLE;
        halSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        halSPI.Init.CRCPolynomial = 7;
        halSPI.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        halSPI.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
        halSPI.Init.NSS = SPI_NSS_SOFT;
    }
}

/**
 * Constructs a sending only SPI object
 *
 * @param CSPins an array of chip select pins for selecting which device to communicate with.
 * @param pinLength the number of pins in the chip select array
 * @param sckPin the pin for the clk line
 * @param mosiPin the mosi pin for sending data
 */
SPIf3xx::SPIf3xx(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin) : SPI(CSPins, pinLength, sckPin, mosiPin) {
    uint8_t mosiPort = getMOSIPortID(mosiPin);
    uint8_t sckPort = getSCKPortID(sckPin);
    GPIO_InitTypeDef GPIOInit = {0};
    uint8_t altId = 0x00U;

    if (mosiPort == sckPort) {
        switch (mosiPort) {
#ifdef STM32F334x8
        case 1:
            halSPI.Instance = SPI1;
            if (!__HAL_RCC_SPI1_IS_CLK_ENABLED()) {
                __HAL_RCC_SPI1_CLK_ENABLE();
            }
            altId = GPIO_AF5_SPI1;
            break;
#endif
#ifdef STM32F302x8
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
            altId = GPIO_AF6_SPI3;
            break;
#endif
        default:
            break;
        }

        Pin spiPins[] = {mosiPin, sckPin};
        uint8_t numOfPins = 2;

        GPIOf3xx::gpioStateInit(&GPIOInit, spiPins, numOfPins, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, altId);

        halSPI.Init.Mode = SPI_MODE_MASTER;
        halSPI.Init.Direction = SPI_DIRECTION_1LINE;
        halSPI.Init.DataSize = SPI_DATASIZE_8BIT;

        // advanced settings
        halSPI.Init.TIMode = SPI_TIMODE_DISABLE;
        halSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        halSPI.Init.CRCPolynomial = 7;
        halSPI.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        halSPI.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
        halSPI.Init.NSS = SPI_NSS_SOFT;
    }
}

/**
 * Configures the SPI transmit mode
 *
 * @param baudRate the baudrate to transmit at (4MHz to 31.25KHz)
 * @param mode The SPIMode to use when sending (0-3)
 * @param order MSB first or LSB first
 */
void SPIf3xx::configureSPI(uint32_t baudRate, uint8_t mode, uint8_t order) {
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
    if (order) {
        halSPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
    } else {
        halSPI.Init.FirstBit = SPI_FIRSTBIT_LSB;
    }

    HAL_SPI_Init(&halSPI);
}

/**
 * Toggles a GPIO pin's state
 *
 * @param pin the pin to toggle the state of
 */
void SPIf3xx::togglePin(GPIO* pin) {
    switch (pin->readPin()) {
    case GPIO::State::HIGH:
        pin->writePin(GPIO::State::LOW);
        break;
    case GPIO::State::LOW:
        pin->writePin(GPIO::State::HIGH);
        break;
    }
}

/**
 * Toggle the state of the chip select pin of a device at the start of a transmission.
 *
 * @param device device the device index in the CSPins
 * @return true if valid device, false if device not in CSPins
 */
bool SPIf3xx::startTransmission(uint8_t device) {
    if (device < CSPinsLength) {
        togglePin(CSPins[device]);
        return true;
    }
    return false;
}

/**
 * Toggle the state of the chip select pin of a device back at the end of a transmission.
 *
 * @param device the device index in the CSPins
 * @return true if valid device, false if device not in CSPins
 */
bool SPIf3xx::endTransmission(uint8_t device) {
    if (device < CSPinsLength) {
        togglePin(CSPins[device]);
        return true;
    }
    return false;
}

/**
 * Writes a single byte out to the SPI device.
 *
 * @param byte the byte to write
 */
SPI::SPIStatus SPIf3xx::write(uint8_t byte) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Transmit(&halSPI, &byte, 1, DEFAULT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

/**
 * Reads a single byte from a SPI device.
 *
 * @return the byte read
 */
SPI::SPIStatus SPIf3xx::read(uint8_t* out) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Receive(&halSPI, out, 1, DEFAULT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

/**
 * Writes an array of bytes to the SPI device.
 *
 * @param device the device to write to in CSPins
 * @param bytes an array of bytes of length n to write to SPI device
 * @param length the length of the array
 */
SPI::SPIStatus SPIf3xx::write(uint8_t* bytes, uint8_t length) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Transmit(&halSPI, bytes, length, DEFAULT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

/**
 * Reads an array of bytes from a SPI device.
 *
 * @param device the device to write to in CSPins
 * @param bytes an array of length n to receive the bytes from an SPI device
 * @param length the number of bytes to recive
 */
SPI::SPIStatus SPIf3xx::read(uint8_t* bytes, uint8_t length) {
    HAL_StatusTypeDef halStatus = HAL_SPI_Receive(&halSPI, bytes, length, DEFAULT_SPI_TIMEOUT);

    return halToSPIStatus(halStatus);
}

SPI::SPIStatus SPIf3xx::halToSPIStatus(HAL_StatusTypeDef halStatus) {
    switch (halStatus) {
    case HAL_OK: return SPIStatus::OK;
    case HAL_ERROR: return SPIStatus::ERROR;
    case HAL_BUSY: return SPIStatus::BUSY;
    case HAL_TIMEOUT: return SPIStatus::TIMEOUT;
    }
}
}// namespace EVT::core::IO
