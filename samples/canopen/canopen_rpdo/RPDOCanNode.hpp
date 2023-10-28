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
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 22;

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
        {// Device Type
         .Key = CO_KEY(0x1000, 0x00, CO_OBJ_____R_),
         .Type = CO_TUNSIGNED32,
         .Data = (CO_DATA) 0x00
        },
        {// Error Register
         .Key = CO_KEY(0x1001, 0x00, CO_OBJ_____R_),
         .Type = CO_TUNSIGNED8,
         .Data = (CO_DATA) 0x00
        },
        {// Sync ID, defaults to 0x80
         .Key = CO_KEY(0x1005, 0x00, CO_OBJ_DN__R_),
         .Type = CO_TUNSIGNED32,
         .Data = (CO_DATA) 0x80
        },
        {// COB-ID EMCY
         .Key = CO_KEY(0x1014, 0x00, CO_OBJ__N__R_),
         .Type = CO_TEMCY_ID,
         .Data = (CO_DATA) 0x80
        },
        {// Heartbeat Producer
         .Key = CO_KEY(0x1017, 0x00, CO_OBJ_D___R_),
         .Type = CO_THB_PROD,
         .Data = (CO_DATA) 2000
        },

        /*
         * Identity Object
         * 0. Object Start Key
         * 1. Vendor ID
         * 2. Product Code
         * 3. Revision Number
         * 4. Serial Number
         */
        {// Identity Object
         .Key = CO_KEY(0x1018, 0x00, CO_OBJ_D___R_),
         .Type = CO_TUNSIGNED8,
         .Data = (CO_DATA) 0x04
        },
        {// Vendor ID
         .Key = CO_KEY(0x1018, 0x01, CO_OBJ_____R_),
         .Type = CO_TUNSIGNED8,
         .Data = (CO_DATA) 0x00
        },
        {// Product Code
         .Key = CO_KEY(0x1018, 0x02, CO_OBJ_____R_),
         .Type = CO_TUNSIGNED8,
         .Data = (CO_DATA) 0x00
        },
        {// Revision Number
         .Key = CO_KEY(0x1018, 0x03, CO_OBJ_____R_),
         .Type = CO_TUNSIGNED8,
         .Data = (CO_DATA) 0x00
        },
        {// Serial number
         .Key = CO_KEY(0x1018, 0x04, CO_OBJ_____R_),
         .Type = CO_TUNSIGNED8,
         .Data = (CO_DATA) 0x00
        },

        // SDO CAN message IDS.
        // 1: Client -> Server ID, default is 0x600 + NODE_ID
        // 2: Server -> Client ID, default is 0x580 + NODE_ID
        {
            // Communication Object SDO Server
            .Key = CO_KEY(0x1200, 0x00, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) 0x02,
        },
        {
            // SDO Server Request COBID
            .Key = CO_KEY(0x1200, 0x01, CO_OBJ__N__R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_COBID_SDO_REQUEST(),
        },
        {
            // SDO Server Response COBID
            .Key = CO_KEY(0x1200, 0x02, CO_OBJ__N__R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_COBID_SDO_RESPONSE(),
        },

        // RPDO settings
        // 0: RPDO number in index and total number of sub indexes.
        // 1: The COB-ID to receive PDOs from.
        // 2: transmission trigger
        {
            // RPDO #0 Mapping Object
            .Key = CO_KEY(0x1400, 0x00, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED8,
            .Data = (CO_DATA) 0x02,
        },
        {
            // COB-ID used by TPDO
            // 180h+TPDO Node-ID
            .Key = CO_KEY(0x1400, 0x01, CO_OBJ_D___R_),
            .Type = CO_TPDO_ID,
            .Data = (CO_DATA) CO_COBID_TPDO_DEFAULT(0) + TPDO_NODE_ID,
        },
        {
            // Transmission type
            .Key = CO_KEY(0x1400, 0x02, CO_OBJ_D___R_),
            .Type = CO_TPDO_TYPE,
            .Data = (CO_DATA) 0xFE,// asynchronous trigger
        },

        // RPDO0 mapping, determines the PDO messages to send when RPDO0 is triggered
        // 0: The number of PDO message associated with the RPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            // RPDO #0 Mapping Object
            .Key = CO_KEY(0x1600, 0x00, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED8,
            .Data = (CO_DATA) 0x02,
        },
        {
            // link the first byte to (0x2100, 0, 8) - sampleDataA
            .Key = CO_KEY(0x1600, 0x01, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_LINK(0x2100, 0x01, 8),
        },
        {
            // link the second byte to (0x2100, 1, 16) - sampleDataB
            .Key = CO_KEY(0x1600, 0x02, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_LINK(0x2100, 0x02, 16),
        },

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO
        {
            .Key = CO_KEY(0x2100, 0, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED8,
            .Data = (CO_DATA) 2,
        },
        {
            // sampleDataA
            .Key = CO_KEY(0x2100, 1, CO_OBJ____PRW),
            .Type = CO_TUNSIGNED8,
            .Data = (uintptr_t) &sampleDataA,
        },
        {
            // sampleDataB
            .Key = CO_KEY(0x2100, 2, CO_OBJ____PRW),
            .Type = CO_TUNSIGNED16,
            .Data = (uintptr_t) &sampleDataB,
        },

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK};
};
