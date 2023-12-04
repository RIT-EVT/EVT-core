#ifndef EVT_CANDEVICE_HPP
#define EVT_CANDEVICE_HPP

#include "co_obj.h"

/**
 * An abstract class used to define a class as a participant in the CAN Network.
 * Provides functions for getting the object dictionary, its size and the ID
 * of the CANDevice. These are used to initialize our CAN settings by just
 * passing a reference to the CANDevice subclass.
 */
class CANDevice {
public:
    /**
     * Get a pointer to the object dictionary
     *
     * @return Pointer to the start of the object dictionary
     */
    virtual CO_OBJ_T* getObjectDictionary() = 0;

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    virtual uint8_t getNumElements() = 0;

    /**
    * Get the device's node ID
    *
    * @return The node ID of the can device.
     */
    virtual uint8_t getNodeID() = 0;
};

#endif//EVT_CANDEVICE_HPP
