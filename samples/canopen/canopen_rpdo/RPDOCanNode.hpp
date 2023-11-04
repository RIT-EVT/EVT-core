/**
 * Representation of the CAN node. Handles constructing the object
 * dictionary and other baseline settings. The idea is that each "board"
 * will have a specific object dictionary associated with it. The object
 * dictionary itself will also need to have information on "data of interest".
 * For example, a temperature management system may to expose water pump
 * flow rate in the object dictionary.
 */
#include <stdint.h>

#include <co_core.h>
#include <EVT/io/CANOpenMacros.hpp>

class RPDOCanNode {
public:
    RPDOCanNode();

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
    void setSampleDataA(uint8_t newValue);
    void setSampleDataB(uint16_t newValue);

    /**
     * Get the contained sample data
     *
     * @return The value of the sample data
     */
    uint8_t getSampleDataA();
    uint16_t getSampleDataB();

    /**
     * Get a pointer to the start of the object dictionary
     *
     * @return Pointer to the start of the object dictionary
     */
    CO_OBJ_T* getObjectDictionary();

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    uint8_t getNumElements();

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0x02;

    static constexpr uint8_t TPDO_NODE_ID = 0x01;

private:
    /**
     * This sample data will be exposed over CAN through the object
     * dictionary. The address of the variable will be included in the
     * object dictionary and can be updated via SDO via a CANopen client.
     * This device will then broadcast the value via a triggered PDO.
     */
    uint8_t sampleDataA;
    uint16_t sampleDataB;

    /**
     * Have to know the size of the object dictionary for initialization
     * process.
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 40;

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        /**
         * Mandatory Identification Keys
         */
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014(0x00),

        /**
         * Heartbeat
         */
        HEARTBEAT_PRODUCER_1017(2000),

        /**
         * Identity Object
         */
        IDENTITY_OBJECT_1018,

        /**
         * SDO Configuration
         */
        SDO_CONFIGURATION_1200,

        /**
         * Asyncronous trigger for RPDO 0 which will receive
         * TPDO 0 of the TPDO_NODE_ID
         */
        RPDO_SETTINGS_OBJECT_140X(0, 0, TPDO_NODE_ID, 0xFE),

        // RPDO0 mapping, determines the PDO messages to send when RPDO0 is triggered
        // 0: The number of PDO message associated with the RPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        RPDO_N_MAPPING_START_KEY_160X(0, 2),
        RPDO_N_MAPPING_ENTRY_N(0, 1, 8),
        RPDO_N_MAPPING_ENTRY_N(0, 2, 16),

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO
        DATA_LINK_START_KEY_210X(0, 2),
        DATA_LINK_210X(0, 1, CO_TUNSIGNED8, &sampleDataA),
        DATA_LINK_210X(0, 2, CO_TUNSIGNED16, &sampleDataB),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK
    };
};
