#include <cstdint>

#include <co_core.h>
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
class TPDOCanNode : public CANDevice {
public:
    TPDOCanNode();

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
    static constexpr uint8_t NODE_ID = 1;

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
//    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 24;
//
//    /**
//     * The object dictionary itself. Will be populated by this object during
//     * construction.
//     *
//     * The plus one is for the special "end of dictionary" marker.
//     */
//    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
//        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
//        HEARTBEAT_PRODUCER_1017(2000),
//        IDENTITY_OBJECT_1018,
//        SDO_CONFIGURATION_1200,
//
//        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),
//
//        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x02),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x01, PDO_MAPPING_UNSIGNED8),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),
//
//        // User defined data, this will be where we put elements that can be
//        // accessed via SDO and depending on configuration PDO
//        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0), 0x02),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0), 0x01, CO_TUNSIGNED8, &sampleDataA),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0), 0x02, CO_TUNSIGNED16, &sampleDataB),
//
//        // End of dictionary marker
//        CO_OBJ_DICT_ENDMARK,
//    };
//};



    uint16_t VCUBoardSig = 1;

    uint16_t battPackCurrent = 0x01;

    uint16_t switchFaultStatus = 0x01;

    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 26;
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 100),

        // TPDO 0 Map
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x02),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO

        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0x00), 0x02),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x00), 0x01, CO_TUNSIGNED16, &battPackCurrent),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x00), 0x02, CO_TUNSIGNED16, &switchFaultStatus),

        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0x100), 0x01),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x100), 0x01, CO_TUNSIGNED16, &VCUBoardSig),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};



/**
#include <cstdint>

#include <co_core.h>
#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>


class TPDOCanNode : public CANDevice {
public:
    TPDOCanNode();

    void setSampleDataA(uint8_t newValue);
    void setSampleDataB(uint16_t newValue);

    uint8_t getSampleDataA();
    uint16_t getSampleDataB();

    void update();


    CO_OBJ_T* getObjectDictionary() override;

    uint8_t getNumElements() override;


    uint8_t getNodeID() override;


    static constexpr uint8_t NODE_ID = 1;
    union switchData_u {
        uint16_t battCurrent = 65535;
        uint16_t hibCurrent;
        uint16_t tmsCurrent;
        uint16_t hudlCurrent;
        uint16_t accCurrent;
        uint16_t gubCurrent;

        int16_t switch0Temp;
        int16_t switch1Temp;
        int16_t switch2Temp;
    };

private:

    uint8_t sampleDataA;
    uint16_t sampleDataB;

    switchData_u PowerSwitchState;


    uint16_t VCUBoardSig = 1;

    uint16_t battPackCurrent = 0x01;

    uint16_t switchFaultStatus = 0x01;

    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 70;
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x00, 0x00, 0, RECEIVE_PDO_TRIGGER_ASYNC),

        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x00, 0x01),
        {
            .Key  = CO_KEY(0x1600 + 0x00, 0x01, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_LINK(0x2200 + 0x00, 0x00 + 0x01, PDO_MAPPING_UNSIGNED16),
        },

        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 100),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x01, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 1000),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x02, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 1000),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x03, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 1000),

        // TPDO 0 Map
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x02),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),

        // TPDO 1 Map
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x01, 0x04),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x01, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x02, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x03, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x04, PDO_MAPPING_UNSIGNED16),

        // TPDO 2 Map
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x02, 0x02),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 0x01, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 0x02, PDO_MAPPING_UNSIGNED16),

        // TPDO 3 Map
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x03, 0x03),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x03, 0x01, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x03, 0x02, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x03, 0x03, PDO_MAPPING_UNSIGNED16),

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO

        DATA_LINK_START_KEY_21XX(0x00, 0x02),
        DATA_LINK_21XX(0x00, 0x01, CO_TUNSIGNED16, &battPackCurrent),
        DATA_LINK_21XX(0x00, 0x02, CO_TUNSIGNED16, &switchFaultStatus),

        DATA_LINK_START_KEY_21XX(0x01, 0x04),
        DATA_LINK_21XX(0x01, 0x01, CO_TUNSIGNED16, &PowerSwitchState.battCurrent),
        DATA_LINK_21XX(0x01, 0x02, CO_TUNSIGNED16, &PowerSwitchState.hibCurrent),
        DATA_LINK_21XX(0x01, 0x03, CO_TUNSIGNED16, &PowerSwitchState.tmsCurrent),
        DATA_LINK_21XX(0x01, 0x04, CO_TUNSIGNED16, &PowerSwitchState.hudlCurrent),

        DATA_LINK_START_KEY_21XX(0x02, 0x02),
        DATA_LINK_21XX(0x02, 0x01, CO_TUNSIGNED16, &PowerSwitchState.accCurrent),
        DATA_LINK_21XX(0x02, 0x02, CO_TUNSIGNED16, &PowerSwitchState.gubCurrent),

        DATA_LINK_START_KEY_21XX(0x03, 0x03),
        DATA_LINK_21XX(0x03, 0x01, CO_TSIGNED16, &PowerSwitchState.switch0Temp),
        DATA_LINK_21XX(0x03, 0x02, CO_TSIGNED16, &PowerSwitchState.switch1Temp),
        DATA_LINK_21XX(0x03, 0x03, CO_TSIGNED16, &PowerSwitchState.switch2Temp),

        DATA_LINK_START_KEY_21XX(0x100, 0x01),
        DATA_LINK_21XX(0x100, 0x01, CO_TUNSIGNED16, &VCUBoardSig),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};



 */
