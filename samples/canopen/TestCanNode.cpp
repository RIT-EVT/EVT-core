#include "TestCanNode.hpp"

TestCanNode::TestCanNode() {
    // Populate the object dictionary
    
    // Counter provided so we don't have to worry about hard coding the index
    // into the object dictionary, makes it easier to insert elements
    uint8_t counter = 0;
    
    // Sync ID, defaults to 0x80
    objectDictionary[counter++] = { 
        .Key = CO_KEY(0x1005, 0, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0x80
    };

    // In order, Vendor ID, Product Code, Revision Number, Serial Number.
    // Place holder data put in for now
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1018, 1, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0x10
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1018, 2, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0x11
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1018, 3, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0x13
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1018, 4, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0x14
    };

    // SDO CAN message IDs
    // By default Client -> Server is 0x600 + NODE_ID
    // By default Server -> Client is 0x580 + NODE_ID
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1200, 1, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0x600 + NODE_ID
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1200, 2, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0x580 + NODE_ID
    };

    // TPDO1 settings
    // The TPDO number, default 0
    // The COB-ID used by TPDO1, provided as function of the TPDO number
    // How the TPDO is triggered, default to manual triggering
    // Inhibit time, defaults to 0
    // Timer trigger time in 1ms units, 0 will disable timer based triggering
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1800, 0, CO_UNSIGNED8|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1800, 1, CO_UNSIGNED32|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)CO_COBID_TPDO_DEFAULT(0)
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1800, 2, CO_UNSIGNED8|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0xFE
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1800, 3, CO_UNSIGNED16|CO_OBJ_D__R_),
        .Type = 0,
        .Data = (uintptr_t)0
    };
    objectDictionary[counter++] = {
        .Key = CO_KEY(0x1800, 5, CO_UNSIGNED16|CO_OBJ_D__R_),
        .Type = CO_TEVENT,
        .Data = (uintptr_t)0
    };
}
