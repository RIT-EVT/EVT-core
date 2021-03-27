#ifndef _EVT_I2CF302X8_HPP_
#define _EVT_I2CF302X8_HPP_

#include <EVT/io/I2C.hpp>
#include <EVT/io/pin.hpp>
#include "GPIOf302x8.hpp"

namespace EVT::core::IO
{

class I2Cf302x8 : public I2C
{
public:

    template<Pin i2c_sda, Pin i2c_scl, unsigned int freq, uint8_t portId>
    static I2Cf302x8 & getInstance();

    /**
    *  write - Writes a string of data to an I2C slave.
    *
    *  Parameters:
    *      addr    - The unshifted 7-bit address of the I2C slave that will
    *                  be written to.
    *      data    - The pointer to the array of bytes that will be written
    *                  to the slave.
    *      length  - The number of bytes that will be written to the slave.
    */
    void write(uint8_t addr, char *data, int length);


    /**
    *  read - Reads a string of data from an I2C slave.
    *
    *  Parameters:
    *      addr    - The unshifted 7-bit address of the I2C slave that will
    *                  be read from.
    *      data    - The pointer to the buffer that will be used to store the
    *                  data recieved from the slave.
    *      length  - The number of bytes that will be read from the slave.
    */
    void read(uint8_t addr, char *data, int length);

private:

    I2C_HandleTypeDef my_i2c;

    // Private constructor and destructor
    I2Cf302x8(Pin i2c_sda, Pin i2c_scl, unsigned int freq, uint8_t portId);
    ~I2Cf302x8();

    constexpr static uint32_t DEFAULT_I2C_FREQ = 100000;
    constexpr static uint32_t DEFAULT_I2C_TIMEOUT = 100;
};

template <Pin i2c_sda, Pin i2c_scl, unsigned int freq, uint8_t portId>
I2Cf302x8 & I2Cf302x8::getInstance()
{
    static I2Cf302x8 instance(i2c_sda, i2c_scl, freq, portId);
    return instance;
}

}

#endif //EVT_I2CF302X8_HPP
