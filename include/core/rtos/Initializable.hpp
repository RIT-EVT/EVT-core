#ifndef _EVT_RTOS_INITIALIZABLE_
#define _EVT_RTOS_INITIALIZABLE_

#include <core/rtos/BytePoolBase.hpp>
#include <core/rtos/Enums.hpp>
#include <tx_api.h>

#ifndef INITIALIZABLE_NAME_MAX_LENGTH
    #define INITIALIZABLE_NAME_MAX_LENGTH 26
#endif //INITIALIZABLE_NAME_MAX_LENGTH

namespace core::rtos {

/**
 * Abstract class that represents a ThreadX wrapper object that must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 */
class Initializable {
public:
    /**
     * Initialize this object with the kernel
     *
     * @param pool The byte pool that this initializable object will be stored in
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    virtual TXError init(BytePoolBase& pool) = 0;

    /**
     * Copy the name of this Initializable object into the character array pointed to by destination &
     * insert a null-terminating character at the end of the given array for safety
     *
     * @param[out] destination Character array to copy the name into.
     * Should be INITIALIZABLE_NAME_MAX_LENGTH bytes long
     * @param[in] size the size of the output array. Should be INITIALIZABLE_NAME_MAX_LENGTH bytes, unless you
     * want the name to be truncated. If the given size is larger than INITIALIZABLE_NAME_MAX_LENGTH, it will be
     * set to INITIALIZABLE_NAME_MAX_LENGTH
     */
    void getName(char* destination, size_t size);

protected:

    /**
     * Initializable constructor
     * @param[in] name pointer to the name of the Initializable. The first INITIALIZABLE_NAME_MAX_LENGTH bytes of the
     * name will be copied into the local name array.
     */
    explicit Initializable(char *name);

    /**
     * The name of this initializable object
     */
    char name[INITIALIZABLE_NAME_MAX_LENGTH];
};

} // namespace core::rtos

#endif //_EVT_RTOS_INITIALIZABLE_
