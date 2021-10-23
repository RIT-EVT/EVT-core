#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f3xx/f302x8/I2Cf302x8.hpp>

namespace EVT::core::IO {

/**
 * Get the I2C ID associated that is associated with the given pin
 * combination. This information is pulled from the STM32F302x8
 * documentation with easier documentation on MBed's STM32F302r8
 * documentation.
 *
 * TODO: Make modifications for this to be runnable at compile time
 * allowing for validation of the selected pins.
 *
 * @param sclPin The selected I2C clock pin.
 * @return
 */
static uint8_t getPortID(Pin sclPin) {
    switch (sclPin) {
        case Pin::PB_8:
        case Pin::PB_6:
        case Pin::PA_15:
            return 1;
        case Pin::PA_9:
        case Pin::PF_1:
            return 2;
        case Pin::PA_8:
            return 3;
        default:
            return 0;
    }
    return 0;  // Should not get here, unless bad pin given
}

I2Cf302x8::I2Cf302x8(Pin sclPin, Pin sdaPin) : I2C(sclPin, sdaPin) {
        GPIO_InitTypeDef gpioInit;

    uint8_t portId = getPortID(sclPin);
    switch (portId) {
        case 1:
            halI2C.Instance = I2C1;

            if (!(__HAL_RCC_I2C1_IS_CLK_ENABLED()))
                __HAL_RCC_I2C1_CLK_ENABLE();

            gpioInit.Alternate = GPIO_AF4_I2C1;

            break;
        case 2:
            halI2C.Instance = I2C2;

            if (!(__HAL_RCC_I2C2_IS_CLK_ENABLED()))
                __HAL_RCC_I2C2_CLK_ENABLE();

            gpioInit.Alternate = GPIO_AF4_I2C2;

            break;
        case 3:
            halI2C.Instance = I2C3;

            if (!(__HAL_RCC_I2C3_IS_CLK_ENABLED()))
                __HAL_RCC_I2C3_CLK_ENABLE();

            gpioInit.Alternate = GPIO_AF2_I2C3;
            break;
        default:
            break;
    }

    Pin i2cPins[2]   = {sclPin, sdaPin};

    gpioInit.Pin = static_cast<uint32_t>(1
            << (static_cast<uint32_t>(i2cPins[0]) & 0x0F)) |
                static_cast<uint32_t>(1
            << (static_cast<uint32_t>(i2cPins[1]) & 0x0F));

    gpioInit.Mode       = GPIO_MODE_AF_OD;
    gpioInit.Pull       = GPIO_NOPULL;
    gpioInit.Speed      = GPIO_SPEED_FREQ_HIGH;

    for (uint8_t i = 0; i < 2; i++) {
        switch ((static_cast<uint8_t>(i2cPins[i]) & 0xF0) >> 4) {
            case 0x0:
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &gpioInit);
                break;
            case 0x1:
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &gpioInit);
                break;
            case 0x2:
                __HAL_RCC_GPIOC_CLK_ENABLE();
                HAL_GPIO_Init(GPIOC, &gpioInit);
                break;
            case 0x3:
                __HAL_RCC_GPIOD_CLK_ENABLE();
                HAL_GPIO_Init(GPIOD, &gpioInit);
                break;
            default:
                break;
        }
    }

    // 8MHz = 0x00310309; 16MHz = 0x10320309; 48MHz = 0x50330309
    halI2C.Init.Timing           = 0x00310309;
    // Timing Register Layout(Bits): [PRESC(4)][RESERVED(4)]
    //                               [SCLDEL(4)][SDADEL(4)]
    //                               [SCLH(8)][SCLL(8)]
    halI2C.Init.OwnAddress1      = 0;
    halI2C.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    halI2C.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    halI2C.Init.OwnAddress2      = 0;
    halI2C.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    halI2C.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    halI2C.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&halI2C);
}

void I2Cf302x8::write(uint8_t addr, uint8_t byte) {
    HAL_I2C_Master_Transmit(&halI2C, addr << 1, &byte, 1,
            DEFAULT_I2C_TIMEOUT);
}

uint8_t I2Cf302x8::read(uint8_t addr) {
    uint8_t data;
    HAL_I2C_Master_Receive(&halI2C, addr << 1, &data, 1,
            DEFAULT_I2C_TIMEOUT);
    return data;
}

/**
 * Override the default multi byte write since the F302x8 supports doing
 * this via the HAL.
 */
void I2Cf302x8::write(uint8_t addr, uint8_t* bytes, uint8_t length) {
    HAL_I2C_Master_Transmit(&halI2C, addr << 1, bytes, length,
            DEFAULT_I2C_TIMEOUT);
}

/**
 * Override the default multi byte read since the F302x8 supports doing
 * this via the HAL.
 */
void I2Cf302x8::read(uint8_t addr, uint8_t* bytes, uint8_t length) {
    HAL_I2C_Master_Receive(&halI2C, addr << 1, bytes, length,
            DEFAULT_I2C_TIMEOUT);
}

void I2Cf302x8::writeMemReg(uint8_t addr, uint32_t memAddress, uint8_t byte, uint16_t memAddSize) {
    uint16_t memAddress16 = memAddress;
    HAL_I2C_Mem_Write(&halI2C, addr << 1, memAddress16, memAddSize, &byte, 1, DEFAULT_I2C_TIMEOUT);
}

uint8_t I2Cf302x8::readMemReg(uint8_t addr, uint32_t memAddress, uint8_t* byte, uint16_t memAddSize) {
    uint16_t memAddress16 = memAddress;
    return HAL_I2C_Mem_Read(&halI2C, addr << 1, memAddress16, memAddSize, byte, 1, DEFAULT_I2C_TIMEOUT);
}

}  // namespace EVT::core::IO
