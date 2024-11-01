#include <cstdint>
#include <core/utils/log.hpp>

#include <co_core.h>
#include <co_csdo.h>
#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>

/**
 * Representation of the CAN node. Handles constructing the object
 * dictionary and other baseline settings. The idea is that each "board"
 * will have a specific object dictionary associated with it. The object
 * dictionary itself will also need to have information on "data of interest".
 * For example, a temperature management system may to expose water pump
 * flow rate in the object dictionary.
 */

namespace log = core::log;

class SDOCanNode : public CANDevice {
public:
    SDOCanNode();

    /**
     * Update Object Dictionary entry
     *
     * @param newValue[in] The value to set sample data to
     */
    void SDOTransfer(CO_NODE node);

    /**
     * Read Object Dictionary entry
     *
     * @param newValue[in] The value to set sample data to
     */
    void SDOReceive(CO_NODE node);

    /**
     * Get the contained sample data
     *
     * @return The value of the sample data
     */
    uint8_t getSampleDataA();
    uint16_t getSampleDataB();

    /**
     * increments counters up
     */
    void update();

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
    /**
     * This sample data will be exposed over CAN through the object
     * dictionary. The address of the variable will be included in the
     * object dictionary and can be updated via SDO via a CANopen client.
     * This device will then broadcast the value via a triggered PDO.
     */
    uint8_t sampleDataA;
    uint8_t sampleDataB;

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

        {
            /* Communication Object SDO Server */
            .Key  = CO_KEY(0x1280, 0x00, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) 0x03,
        },
        {
            /* SDO Server Request COBID */
            .Key  = CO_KEY(0x1280, 0x01, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_COBID_SDO_REQUEST(),
        },
        { /* SDO Server Response COBID */
            .Key  = CO_KEY(0x1280, 0x02, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_COBID_SDO_RESPONSE(),
        },
        {
            .Key  = CO_KEY(0x1280, 0x03, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED8,
            .Data = (CO_DATA) 1,
        },

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO
        DATA_LINK_START_KEY_21XX(0, 0x02),
        DATA_LINK_21XX(0x00, 0x01, CO_TUNSIGNED8, &sampleDataA),
        DATA_LINK_21XX(0x00, 0x02, CO_TUNSIGNED8, &sampleDataB),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};
void AppCSdoFinishCb(CO_CSDO *csdo, uint16_t index, uint8_t sub, uint32_t code);