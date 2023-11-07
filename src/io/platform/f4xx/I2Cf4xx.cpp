#include <EVT/io/platform/f4xx/I2Cf4xx.hpp>

#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f4xx/GPIOf4xx.hpp>

//#include <EVT/utils/log.hpp>

//namespace log = EVT::core::log;

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
 * @param sclPin The selected I2C clock pin
 * @return The port ID associated with the selected pins
 */
static uint8_t getPortID(Pin sclPin) {
#ifdef STM32F302x8

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
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid SCL Pin");
        return 0;
    }
#endif

#ifdef STM32F334x8
    return 1;
#endif

#ifdef STM32f446xx
    switch (sclPin) {
    case Pin::PB_6:
        return 1;
    case Pin::PB_10:
        return 2;
    default:
//        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid SCL Pin");
        return 0;
    }
#endif
}

/**
 * Get the target I2C instance based on the version of the STM32f3
 *
 * The STM32F302r8 supports 3 I2C instances, the STM32F334x8 supports
 * 1 I2C instance
 *
 * @param portId The target port, for the STM32f334 this is always 1
 */
static void getInstance(uint8_t portID, I2C_TypeDef** instance, uint8_t* altId) {
#ifdef STM32F302x8
    switch (portID) {
    case 1:
        *instance = I2C1;

        if (!__HAL_RCC_I2C1_IS_CLK_ENABLED())
            __HAL_RCC_I2C1_CLK_ENABLE();

        *altId = GPIO_AF4_I2C1;

        break;
    case 2:
        *instance = I2C2;

        if (!__HAL_RCC_I2C2_IS_CLK_ENABLED())
            __HAL_RCC_I2C2_CLK_ENABLE();

        *altId = GPIO_AF4_I2C2;

        break;
    case 3:
        *instance = I2C3;

        if (!__HAL_RCC_I2C3_IS_CLK_ENABLED())
            __HAL_RCC_I2C3_CLK_ENABLE();

        *altId = GPIO_AF2_I2C3;
        break;
    default:
        break;
    }
#endif

#ifdef STM32F334x8
    *instance = I2C1;
    *altId = GPIO_AF4_I2C1;
    if (!(__HAL_RCC_I2C1_IS_CLK_ENABLED()))
        __HAL_RCC_I2C1_CLK_ENABLE();
#endif

#ifdef STM32f446xx
    switch(portID){
    case 1:
        *instance = I2C1;

        if (!__HAL_RCC_I2C1_IS_CLK_ENABLED())
            __HAL_RCC_I2C1_CLK_ENABLE();

        *altId = GPIO_AF4_I2C1;
        break;
    case 2:
        *instance = I2C2;

        if (!__HAL_RCC_I2C2_IS_CLK_ENABLED())
            __HAL_RCC_I2C2_CLK_ENABLE();

        *altId = GPIO_AF4_I2C2;

        break;
    default:
        break;
    }
#endif
}

I2Cf4xx::I2Cf4xx(Pin sclPin, Pin sdaPin) : I2C(sclPin, sdaPin) {
    GPIO_InitTypeDef gpioInit;
    uint8_t altId = 0x00U;

    uint8_t portId = getPortID(sclPin);
    getInstance(portId, &halI2C.Instance, &altId);

    Pin i2cPins[] = {sclPin, sdaPin};
    uint8_t numOfPins = 2;

    GPIOf4xx::gpioStateInit(&gpioInit, i2cPins, numOfPins, GPIO_MODE_AF_OD,
                            GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, altId);

    // 8MHz = 0x00310309; 16MHz = 0x10320309; 48MHz = 0x50330309
    halI2C.Init.ClockSpeed = 0x00310309;
    // Timing Register Layout(Bits): [PRESC(4)][RESERVED(4)]
    //                               [SCLDEL(4)][SDADEL(4)]
    //                               [SCLH(8)][SCLL(8)]
    halI2C.Init.DutyCycle = I2C_DUTYCYCLE_2;
    halI2C.Init.OwnAddress1 = 0;
    halI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    halI2C.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    halI2C.Init.OwnAddress2 = 0;
//    part of F3 but not F4???
//    halI2C.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    halI2C.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    halI2C.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&halI2C);
}

I2C::I2CStatus I2Cf4xx::write(uint8_t addr, uint8_t byte) {
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&halI2C, addr << 1,
                                                       &byte, 1,
                                                       EVT_I2C_TIMEOUT);
    return halToI2CStatus(status);
}

I2C::I2CStatus I2Cf4xx::read(uint8_t addr, uint8_t* output) {
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&halI2C, addr << 1,
                                                      output, 1,
                                                      EVT_I2C_TIMEOUT);
    return halToI2CStatus(status);
}

/**
 * Override the default multi byte write since the F302x8 supports doing
 * this via the HAL.
 */
I2C::I2CStatus I2Cf4xx::write(uint8_t addr, uint8_t* bytes, uint8_t length) {
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&halI2C, addr << 1,
                                                       bytes, length,
                                                       EVT_I2C_TIMEOUT);
    return halToI2CStatus(status);
}

/**
 * Override the default multi byte read since the F302x8 supports doing
 * this via the HAL.
 */
I2C::I2CStatus I2Cf4xx::read(uint8_t addr, uint8_t* bytes, uint8_t length) {
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&halI2C, addr << 1,
                                                      bytes, length,
                                                      EVT_I2C_TIMEOUT);
    return halToI2CStatus(status);
}

I2C::I2CStatus I2Cf4xx::writeMemReg(uint8_t addr, uint32_t memAddress,
                                    uint8_t byte, uint16_t memAddSize,
                                    uint8_t maxWriteTime) {
    uint16_t memAddress16 = memAddress;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&halI2C, addr << 1,
                                                 memAddress16, memAddSize,
                                                 &byte, 1, EVT_I2C_TIMEOUT);
    HAL_Delay(maxWriteTime);
    return halToI2CStatus(status);
}

I2C::I2CStatus I2Cf4xx::readMemReg(uint8_t addr, uint32_t memAddress,
                                   uint8_t* byte,
                                   uint16_t memAddSize) {
    uint16_t memAddress16 = memAddress;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&halI2C, addr << 1,
                                                memAddress16, memAddSize, byte,
                                                1, EVT_I2C_TIMEOUT);
    return halToI2CStatus(status);
}

I2C::I2CStatus I2Cf4xx::writeMemReg(uint8_t addr, uint32_t memAddress,
                                    uint8_t* bytes, uint8_t size,
                                    uint16_t memAddSize,
                                    uint8_t maxWriteTime) {
    uint16_t memAddress16 = memAddress;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&halI2C, addr << 1,
                                                 memAddress16, memAddSize,
                                                 bytes, size,
                                                 EVT_I2C_TIMEOUT);
    HAL_Delay(maxWriteTime);
    return halToI2CStatus(status);
}

I2C::I2CStatus I2Cf4xx::readMemReg(uint8_t addr, uint32_t memAddress,
                                   uint8_t* bytes, uint8_t size,
                                   uint16_t memAddSize) {
    uint16_t memAddress16 = memAddress;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&halI2C, addr << 1,
                                                memAddress16, memAddSize,
                                                bytes, size,
                                                EVT_I2C_TIMEOUT);
    return halToI2CStatus(status);
}

I2C::I2CStatus I2Cf4xx::halToI2CStatus(HAL_StatusTypeDef halStatus) {
    switch (halStatus) {
    case HAL_OK:
        return I2C::I2CStatus::OK;
    case HAL_ERROR:
        return I2C::I2CStatus::ERROR;
    case HAL_BUSY:
        return I2C::I2CStatus::BUSY;
    case HAL_TIMEOUT:
        return I2C::I2CStatus::TIMEOUT;
    // Should not get here
    default:
        return I2C::I2CStatus::ERROR;
    }
}

}// namespace EVT::core::IO
