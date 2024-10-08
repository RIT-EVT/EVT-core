#include <co_core.h>
#include <cstdint>

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
class TestCanNode : public CANDevice {
public:
    TestCanNode();

    /**
     * Expose a way to programmatically update the sampleData.
     *
     * An example use case could be temperature data that is collected
     * then stored in this object. Under the hood, the object dictionary
     * has the address of the temperature data, so the triggered PDO
     * would also effectively be updated.
     *
     * @param newValue[in] The value to set sample data to
     */
    void setSampleData(uint8_t newValue);

    /**
     * Get the contained sample data
     *
     * @return The value of the sample data
     */
    uint8_t getSampleData();

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

private:
    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 1;

    /**
     * This sample data will be exposed over CAN through the object
     * dictionary. The address of the variable will be included in the
     * object dictionary and can be updated via SDO via a CANopen client.
     * This device will then broadcast the value via a triggered PDO.
     */
    uint8_t sampleData;

    /**
     * Have to know the size of the object dictionary for initialization
     * process.
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 21;

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 200),

        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0, 1),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0, 0x01, PDO_MAPPING_UNSIGNED8),

        DATA_LINK_START_KEY_21XX(0, 1),
        DATA_LINK_21XX(0, 1, CO_TUNSIGNED8, &sampleData),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};
