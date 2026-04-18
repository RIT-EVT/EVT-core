#include <co_core.h>
#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>
#include <core/io/CANopen.hpp>

namespace io = core::io;

/**
 * Representation of the CAN node. Handles constructing the object
 * dictionary and other baseline settings. The idea is that each "board"
 * will have a specific object dictionary associated with it. The object
 * dictionary itself will also need to have information on "data of interest".
 * For example, a temperature management system may to expose water pump
 * flow rate in the object dictionary.
 */
class SDOCanNode : public CANDevice {
public:
    SDOCanNode(CO_NODE& canNode);

    /**
     * Update Object Dictionary entry
     *
     * @param callback[in] Callback function for the transfer operation
     * @param context[in] Context for the callback function
     */
    void transferData(io::csdo_callback_t callback, void* context);

    /**
     * Read Object Dictionary entry
     *
     * @param callback[in] Callback function for the receive operation
     * @param context[in] Context for the callback function
     */
    void receiveData(io::csdo_callback_t callback, void* context);

    /**
     * Get a pointer to the start of the object dictionary
     *
     * @return Pointer to the start of the object dictionary
     */
    CO_OBJ_T* getObjectDictionary() override;

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    uint8_t getNumElements() override;

    /**
     * Get the device's node ID
     *
     * @return The node ID of the can device.
     */
    uint8_t getNodeID() override;

    /**
     * Get the device's node ID
     *
     * @return The node ID of the can device.
     */
    static constexpr uint8_t NODE_ID = 2;

private:
    /** Holds the data to be transferred */
    uint8_t* transferBuff;

    CO_NODE& node;

    /**
     * Have to know the size of the object dictionary for initialization
     * process.
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 24;

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,
        SDO_CONFIGURATION_1280(1),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};