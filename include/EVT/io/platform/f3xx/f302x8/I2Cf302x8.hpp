#ifndef _EVT_I2Cf302x8_
#define _EVT_I2Cf302x8_

#include <stdint.h>

#include <EVT/io/I2C.hpp>
#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class I2Cf302x8 : public I2C {
public:
    /**
     * Make an instance of an I2C interface for the F3. Will determine
     * which I2C bus of the STM to use based on the provided pins.
     *
     * @param sclPin[in] The I2C clock pin
     * @param sdaPin[in] The I2C data pin
     */
    I2Cf302x8(Pin sclPin, Pin sdaPin);

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
    /** Timeout in milliseconds **/
    constexpr static uint32_t DEFAULT_I2C_TIMEOUT = 100;
    /** Interface into the HAL */
    I2C_HandleTypeDef halI2C;

    /**
     * Convert the STM HAL status to an I2C::I2CStatus
     *
     * @param The HAL status
     * @return The I2C::I2CStatus
     */
    I2C::I2CStatus halToI2CStatus(HAL_StatusTypeDef halStatus);
};

}// namespace EVT::core::IO

#endif
