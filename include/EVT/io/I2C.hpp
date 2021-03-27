#ifndef _EVT_I2C_HPP_
#define _EVT_I2C_HPP_

#include <cstdint>

namespace EVT::core::I2C {

class I2C {
public:

    // Empty constructor
    I2C() {}

    // Empty destructor
    virtual ~I2C() {}

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
    virtual void write(uint8_t addr, char *data, int length) = 0;

    /**
     *  read - Reads a string of data from an I2C slave.
     *
     *  Parameters:
     *      addr    - The unshifted 7-bit address of the I2C slave that will
     *                  be read from.
     *      data    - The pointer to the buffer that will be used to store the
     *                  data received from the slave.
     *      length  - The number of bytes that will be read from the slave.
     */
    virtual void read(uint8_t addr, char *data, int length) = 0;

}; // class I2C

} // namespace EVT::core::I2C

#endif //_EVT_I2C_HPP_
