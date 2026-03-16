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
    typedef union {
        struct {
            uint16_t LVSS_out_HVCurrent[2];
            uint16_t LVSS_out_PowerSwitchCurrents[4];
            uint16_t LVSS_out_Temperatures[2];
            uint16_t LVSS_out_PowerSwitchErrorStatus[3];
        };
        uint16_t outputs[11];
    } LvssData_t;

    LvssData_t outData = {{{0, 1}, {2, 3, 4, 5}, {6, 7}, {8, 9, 10}}};

    static constexpr uint8_t OBJECT_DICTIONARY_SIZE       = 48;
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        ///////////////////////////////////////////////////////////////////////////
        // TPDO0 (LVSS TPDO #0): 2 × u16  (4 bytes)
        ///////////////////////////////////////////////////////////////////////////
//        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 200),
//        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x04),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x03, PDO_MAPPING_UNSIGNED16),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x04, PDO_MAPPING_UNSIGNED16),

        ///////////////////////////////////////////////////////////////////////////
        // TPDO1 (LVSS TPDO #1): 4 × u16  (8 bytes)
        ///////////////////////////////////////////////////////////////////////////
//        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x01, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 400),
//        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x01, 0x01),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x01, PDO_MAPPING_UNSIGNED16),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x02, PDO_MAPPING_UNSIGNED16),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x03, PDO_MAPPING_UNSIGNED16),
//        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 0x04, PDO_MAPPING_UNSIGNED16),

        ///////////////////////////////////////////////////////////////////////////
        // TPDO2 (LVSS TPDO #2): 2 × u16  (4 bytes)
        ///////////////////////////////////////////////////////////////////////////
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x02, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 600),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x02, 0x02),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 0x01, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x02, 0x02, PDO_MAPPING_UNSIGNED16),

        ///////////////////////////////////////////////////////////////////////////
        // TPDO3 (LVSS TPDO #3): 3 × u16  (6 bytes)
        ///////////////////////////////////////////////////////////////////////////
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x03, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 800),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x03, 0x03),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x03, 0x01, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x03, 0x02, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x03, 0x03, PDO_MAPPING_UNSIGNED16),

        ///////////////////////////////////////////////////////////////////////////
        // Data links: what those mapped entries point to in memory
        // (Group numbers 0x00..0x03 correspond to the TPDO number)
        ///////////////////////////////////////////////////////////////////////////

        // TPDO0 payload: HV Current Data (2×u16)
//        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0x00), 0x04),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x00), 0x01, CO_TUNSIGNED16, &outData.LVSS_out_HVCurrent[0]),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x00), 0x02, CO_TUNSIGNED16, &outData.LVSS_out_HVCurrent[1]),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x00), 0x03, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchCurrents[0]),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x00), 0x04, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchCurrents[1]),

        // TPDO1 payload: Power Switch Currents Data (4×u16)
//        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0x01), 0x01),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x01), 0x01, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchCurrents[0]),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x01), 0x02, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchCurrents[1]),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x01), 0x03, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchCurrents[2]),
//        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x01), 0x04, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchCurrents[3]),

//         TPDO2 payload: Temperature Data (2×u16)
        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0x02), 0x02),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x02), 0x01, CO_TUNSIGNED16, &outData.LVSS_out_Temperatures[0]),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x02), 0x02, CO_TUNSIGNED16, &outData.LVSS_out_Temperatures[1]),

        // TPDO3 payload: Power Switch Error Status (3×u16)
        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0x03), 0x03),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x03), 0x01, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchErrorStatus[0]),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x03), 0x02, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchErrorStatus[1]),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x03), 0x03, CO_TUNSIGNED16, &outData.LVSS_out_PowerSwitchErrorStatus[2]),

        CO_OBJ_DICT_ENDMARK,
    };
};
