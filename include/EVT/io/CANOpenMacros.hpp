#ifndef EVT_CANOPENMACROS_HPP
#define EVT_CANOPENMACROS_HPP

#include <co_core.h>

/*
 * Mandatory Identification Key Macros
 */
#define MANDATORY_IDENTIFICATION_ENTRIES_1000_1014(DEVICE_TYPE) \
    {                                                           \
        .Key = CO_KEY(0x1000, 0x00, CO_OBJ_____R_),             \
        .Type = CO_TUNSIGNED32,                                 \
        .Data = (CO_DATA) DEVICE_TYPE                           \
    },                                                          \
    {                                                           \
        .Key = CO_KEY(0x1001, 0x00, CO_OBJ_____R_),             \
        .Type = CO_TUNSIGNED32,                                 \
        .Data = (CO_DATA) 0x00                                  \
    },                                                          \
    {                                                           \
        .Key = CO_KEY(0x1005, 0x00, CO_OBJ_DN__R_),             \
        .Type = CO_TUNSIGNED32,                                 \
        .Data = (CO_DATA) 0x80                                  \
    },                                                          \
    {                                                           \
        .Key = CO_KEY(0x1014, 0x00, CO_OBJ__N__R_),             \
        .Type = CO_TEMCY_ID,                                    \
        .Data = (CO_DATA) 0x80                                  \
    }

/**
 * Heartbeat related macros
 */
// TODO: Implement a heartbeat consumer

#define HEARTBEAT_PRODUCER_1017(INTERVAL)               \
    {                                                   \
        .Key = CO_KEY(0x1017, 0x00, CO_OBJ_D___R_),     \
        .Type = CO_THB_PROD,                            \
        .Data = (CO_DATA) 2000                          \
    }

/*
 * Identity Object
 * 0. Object Start Key
 * 1. Vendor ID
 * 2. Product Code
 * 3. Revision Number
 * 4. Serial Number
 */
#define IDENTITY_OBJECT_1018                            \
    { /* Object Start Key */                            \
         .Key = CO_KEY(0x1018, 0x00, CO_OBJ_D___R_),    \
         .Type = CO_TUNSIGNED8,                         \
         .Data = (CO_DATA) 0x04                         \
    },                                                  \
    { /* Vendor ID */                                   \
         .Key = CO_KEY(0x1018, 0x01, CO_OBJ_____R_),    \
         .Type = CO_TUNSIGNED8,                         \
         .Data = (CO_DATA) 0x00                         \
    },                                                  \
    { /* Product Code */                                \
         .Key = CO_KEY(0x1018, 0x02, CO_OBJ_____R_),    \
         .Type = CO_TUNSIGNED8,                         \
         .Data = (CO_DATA) 0x00                         \
    },                                                  \
    { /* Revision Number */                             \
         .Key = CO_KEY(0x1018, 0x03, CO_OBJ_____R_),    \
         .Type = CO_TUNSIGNED8,                         \
         .Data = (CO_DATA) 0x00                         \
    },                                                  \
    { /* Serial Number */                               \
        .Key = CO_KEY(0x1018, 0x04, CO_OBJ_____R_),     \
        .Type = CO_TUNSIGNED8,                          \
        .Data = (CO_DATA) 0x00                          \
    }

// SDO CAN message IDS.
// 1: Client -> Server ID, default is 0x600 + NODE_ID
// 2: Server -> Client ID, default is 0x580 + NODE_ID
#define SDO_CONFIGURATION_1200                          \
    { /* Communication Object SDO Server */             \
        .Key = CO_KEY(0x1200, 0x00, CO_OBJ_D___R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) 0x02,                         \
    },                                                  \
    { /* SDO Server Request COBID */                    \
        .Key = CO_KEY(0x1200, 0x01, CO_OBJ__N__R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) CO_COBID_SDO_REQUEST(),       \
    },                                                  \
    { /* SDO Server Response COBID */                   \
        .Key = CO_KEY(0x1200, 0x02, CO_OBJ__N__R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) CO_COBID_SDO_RESPONSE()       \
    }

/**
 * RPDO Settings
 */
#define RPDO_SETTINGS_OBJECT_140X(RPDO_NUMBER, TPDO_NUMBER, TPDO_NODE_ID, TRANSMISSION_TYPE) \
    { /* RPDO #0 Settings Object */                                                          \
        .Key = CO_KEY(0x1400 + RPDO_NUMBER, 0x00, CO_OBJ_D___R_),                            \
        .Type = CO_TUNSIGNED8,                                                               \
        .Data = (CO_DATA) 0x02,                                                              \
    },                                                                                       \
    { /* COB-ID used by TPDO  180h+TPDO Node-ID*/                                            \
        .Key = CO_KEY(0x1400 + RPDO_NUMBER, 0x01, CO_OBJ_D___R_),                            \
        .Type = CO_TPDO_ID,                                                                  \
        .Data = (CO_DATA) CO_COBID_TPDO_DEFAULT(TPDO_NUMBER) + TPDO_NODE_ID,                 \
    },                                                                                       \
    { /* Transmission type */                                                                \
        .Key = CO_KEY(0x1400 + RPDO_NUMBER, 0x02, CO_OBJ_D___R_),                            \
        .Type = CO_TPDO_TYPE,                                                                \
        .Data = (CO_DATA) TRANSMISSION_TYPE                                                  \
    }

/**
 * RPDO Mapping
 */
// RPDO N Mapping Object
#define RPDO_N_MAPPING_START_KEY_160X(RPDO_NUMBER, NUMBER_OF_SUB_INDICES) \
    {                                                                      \
        .Key = CO_KEY(0x1600 + RPDO_NUMBER, 0x00, CO_OBJ_D___R_),          \
        .Type = CO_TUNSIGNED8,                                             \
        .Data = (CO_DATA) NUMBER_OF_SUB_INDICES                            \
    }

// RPDO N Mapping Entry N
#define RPDO_N_MAPPING_ENTRY_N(RPDO_NUMBER, SUB_INDEX, DATA_SIZE)                       \
    {                                                                                   \
        .Key = CO_KEY(0x1600 + RPDO_NUMBER, SUB_INDEX, CO_OBJ_D___R_),                  \
        .Type = CO_TUNSIGNED32,                                                         \
        .Data = (CO_DATA) CO_LINK(0x2100 + RPDO_NUMBER, 0x00  + SUB_INDEX, DATA_SIZE)   \
    }

/**
 * Data Linking
 */
#define DATA_LINK_START_KEY_210X(PDO_NUMBER, NUMBER_OF_SUB_INDICES) \
    {                                                               \
        .Key = CO_KEY(0x2100 + PDO_NUMBER, 0, CO_OBJ_D___R_),       \
        .Type = CO_TUNSIGNED8,                                      \
        .Data = (CO_DATA) NUMBER_OF_SUB_INDICES                     \
    }

#define DATA_LINK_210X(PDO_NUMBER, SUB_INDEX, DATA_TYPE, DATA_POINTER)  \
    {                                                                   \
        .Key = CO_KEY(0x2100 + PDO_NUMBER, SUB_INDEX, CO_OBJ_D___R_),   \
        .Type = DATA_TYPE,                                              \
        .Data = (CO_DATA) DATA_POINTER                                  \
    }


#endif//EVT_CANOPENMACROS_HPP
