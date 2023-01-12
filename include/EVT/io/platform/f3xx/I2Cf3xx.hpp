#ifndef _EVT_I2Cf3xx_
#define _EVT_I2Cf3xx_

#include <stdint.h>

#include <EVT/io/I2C.hpp>
#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class I2Cf3xx : public I2C {
public:
    /**
     * Make an instance of an I2C interface for the F3. Will determine
     * which I2C bus of the STM to use based on the provided pins.
     *
     * @param[in] sclPin The I2C clock pin
     * @param[in] sdaPin The I2C data pin
     */
    I2Cf3xx(Pin sclPin, Pin sdaPin);

    I2C::I2CStatus write(uint8_t addr, uint8_t byte) override;

    I2C::I2CStatus read(uint8_t addr, uint8_t* output) override;

    I2C::I2CStatus write(uint8_t addr, uint8_t* bytes, uint8_t length);

    I2C::I2CStatus read(uint8_t addr, uint8_t* bytes, uint8_t length);

    I2C::I2CStatus writeMemReg(uint8_t addr, uint32_t memAddress,
                               uint8_t byte, uint16_t memAddSize,
                               uint8_t maxWriteTime);

    I2C::I2CStatus readMemReg(uint8_t addr, uint32_t memAddress,
                              uint8_t* byte,
                              uint16_t memAddSize);

    I2C::I2CStatus writeMemReg(uint8_t addr, uint32_t memAddress,
                               uint8_t* bytes, uint8_t size,
                               uint16_t memAddSize,
                               uint8_t maxWriteTime);

    I2C::I2CStatus readMemReg(uint8_t addr, uint32_t memAddress,
                              uint8_t* bytes, uint8_t size,
                              uint16_t memAddSize);

private:
    constexpr static uint32_t DEFAULT_I2C_FREQ = 100000;
    /** Interface into the HAL */
    I2C_HandleTypeDef halI2C;

    /**
     * Convert the STM HAL status to an I2C::I2CStatus
     *
     * @param[in] The HAL status
     * @return The I2C::I2CStatus
     */
    I2C::I2CStatus halToI2CStatus(HAL_StatusTypeDef halStatus);
};

}// namespace EVT::core::IO

#endif
