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
class RPDOCanNode : public CANDevice {
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

    uint16_t getPackCur();
    uint16_t getswitchFault();
    uint16_t getBoardSig();

    uint16_t getIn(int i);
    void zeroIn();
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
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 2;

    static constexpr uint8_t TPDO_NODE_ID = 1;

private:
    /**
     * This sample data will be exposed over CAN through the object
     * dictionary. The address of the variable will be included in the
     * object dictionary and can be updated via SDO via a CANopen client.
     * This device will then broadcast the value via a triggered PDO.
     */
    uint8_t sampleDataA;
    uint16_t sampleDataB;

    uint16_t switchFaultStatus;
    uint16_t packCurrent;

    uint16_t boardSig;

    typedef union {
        struct {
            uint16_t LVSS_out_EnableBoardSignal;            ///< LVSS (out) Determines which boards it will send power to
            uint16_t LVSS_in_PowerSwitchCurrents[6];        ///< LVSS (in)
            uint16_t LVSS_in_VicorCurrent;                  ///< LVSS (in) hv vicor current
            uint16_t LVSS_in_PowerSwitchTemperatures[3];    ///< LVSS (in)
            uint16_t LVSS_in_EnableBoardSignal;             ///< LVSS (in)
        };
        struct {
            uint16_t outputs[1];
            uint16_t inputs[11];
        };
    } AccessoryCanData_t;


    AccessoryCanData_t accessoryCanDataUnsafeBuffer;

    static constexpr uint8_t LVSS_NODE_ID = 1;

    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 59; // TODO: CANopen set size of object dictionary

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

        // RPDOS and data links
        //        want what boards are on (the indiv. *_ON_CAN)
        //        all currents (6)
        //        all temperatures (3)
        //        vicor current (1)
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x00, 0x00, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x01, 0x01, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x02, 0x02, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),

        // 4 currents
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x00, 0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x04, PDO_MAPPING_UNSIGNED16),

        // other 2 currents + vicor current
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x01, 0x03),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x03, PDO_MAPPING_UNSIGNED16),

        // temperatures 0-3 & board-enabled bit-packed value
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x02, 0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x04, PDO_MAPPING_UNSIGNED16),

        // Actual TPDO
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 500),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x01),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),

        // data links
        // LVSS!!!!
        // HV Current Data
        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x00), 0x04),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[1]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[2]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x04, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[3]),

        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x01), 0x03),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x01), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[4]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x01), 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[5]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x01), 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_VicorCurrent),

        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x02), 0x04),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchTemperatures[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchTemperatures[1]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchTemperatures[2]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x04, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_EnableBoardSignal),

        // todo: need to add CANOPEN transmits
        // TPDO Datalinks
        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(0x00), 0x01),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(0x00), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_out_EnableBoardSignal),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};
