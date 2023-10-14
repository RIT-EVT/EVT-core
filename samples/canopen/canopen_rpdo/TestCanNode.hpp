/**
 * Representation of the CAN node. Handles constructing the object
 * dictionary and other baseline settings. The idea is that each "board"
 * will have a specific object dictionary associated with it. The object
 * dictionary itself will also need to have information on "data of interest".
 * For example, a temperature management system may to expose water pump
 * flow rate in the object dictionary.
 */
#include <stdint.h>

#include <Canopen/co_core.h>

class TestCanNode {
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

private:
    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0x02;

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
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 15;

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        // Sync ID, defaults to 0x80
        {CO_KEY(0x1005, 0, CO_UNSIGNED32 | CO_OBJ_D__R_), 0, (uintptr_t) 0x80},

        // Information about the hardware, hard coded sample values for now
        // 1: Vendor ID
        // 2: Product Code
        // 3: Revision Number
        // 4: Serial Number
        {
            .Key = CO_KEY(0x1018, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x10,
        },
        {
            .Key = CO_KEY(0x1018, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x11,
        },
        {
            .Key = CO_KEY(0x1018, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x12,
        },
        {
            .Key = CO_KEY(0x1018, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x13,
        },

        // SDO CAN message IDS.
        // 1: Client -> Server ID, default is 0x600 + NODE_ID
        // 2: Server -> Client ID, default is 0x580 + NODE_ID
        {
            .Key = CO_KEY(0x1200, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x600 + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1200, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0x580 + NODE_ID,
        },

        //RPDO settings
        // 0: RPDO number in index and total number of sub indexes.
        // 1: The COB-ID to receive PDOs from.
        // 2: transmission trigger
        {
            .Key = CO_KEY(0x1400, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 3,
        },
        {
            // 180h+TPDO Node-ID
            .Key = CO_KEY(0x1400, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(0) + 0x01,
        },
        {
            // asynchronous trigger
            .Key = CO_KEY(0x1400, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 0xFE,
        },

        // RPDO0 mapping, determines the PDO messages to send when RPDO1 is triggered
        // 0: The number of PDO message associated with the RPDO
        // 1: Link to the first PDO message
        // n: Link to the nth PDO message
        {
            // maps two objects
            .Key = CO_KEY(0x1600, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = (uintptr_t) 2,
        },
        {
            // link the first byte to (0x2100, 0, 8) - sampleDataA
            .Key = CO_KEY(0x1600, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = CO_LINK(0x2100, 0, 8),
        },
        {
            // link the second byte to (0x2100, 1, 16) - sampleDataB
            .Key = CO_KEY(0x1600, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = 0,
            .Data = CO_LINK(0x2100, 1, 16),
        },

        // User defined data, this will be where we put elements that can be
        // accessed via SDO and depending on configuration PDO
        {
            // sampleDataA
            .Key = CO_KEY(0x2100, 0, CO_UNSIGNED8 | CO_OBJ___PRW),
            .Type = 0,
            .Data = (uintptr_t) &sampleDataA,
        },
        {
            // sampleDataB
            .Key = CO_KEY(0x2100, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = 0,
            .Data = (uintptr_t) &sampleDataB,
        },

        // End of dictionary marker
        CO_OBJ_DIR_ENDMARK};
};
