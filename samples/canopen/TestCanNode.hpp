/**
 * Representation of the CAN node. Handles constructing the object
 * dictionary and other baseline settings. The idea is that each "board"
 * will have a specific object dictionary associated with it. The object
 * dicitonary itself will also need to have information on "data of interest".
 * For example, a temperature management system may to expose water pump
 * flow rate in the object dictionary.
 */
#include <stdint.h>

#include <Canopen/co_core.h>

class TestCanNode {
public:
    TestCanNode();

    /**
     * Expose a way to programmably update the sampleData.
     *
     * An example use case could be temperature data that is collected
     * then stored in this object. Under the hood, the object dicitonary
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
    CO_OBJ_T* getObjectDictionary();

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    uint8_t getNumElements();


private:
    /**
     * The number of elements that will be stored by the object dictionary.
     * This will be known.
     */
    static constexpr uint8_t OBJECT_DIRECTIONARY_SIZE = 15;
    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0x01;

    /**
     * This sample data will be exposed over CAN through the object
     * dictionary. The address of the variable will be included in the
     * object dictionary and can be updated via SDO via a CANopen client.
     * This device will then broadcast the value via a triggered PDO.
     */
    uint8_t sampleData;

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DIRECTIONARY_SIZE + 1];

};
