#ifndef EVT_CANOPENMACROS_HPP
#define EVT_CANOPENMACROS_HPP

#define RECEIVE_PDO_TRIGGER_SYNC 0x00
#define RECEIVE_PDO_TRIGGER_ASYNC 0xFE

#define TRANSMIT_PDO_TRIGGER_TIMER 0xFE

#define PDO_MAPPING_UNSIGNED8 8
#define PDO_MAPPING_UNSIGNED16 16
#define PDO_MAPPING_UNSIGNED32 32
#define PDO_MAPPING_UNSIGNED64 64

#define TRANSMIT_PDO_INHIBIT_TIME_DISABLE 0x00

// TODO: I don't like importing co_core into a macros file. We should discuss importing all the types we need individually.
#include "co_core.h"

/*!
 * @brief MANDATORY IDENTIFICATION OBJECTS
 *
 * This macro helps to create a section of the CANOpen Object dictionary
 * that is known as the Mandatory Identification Keys. These keys sit
 * in the range of 0x1000-0x1014 and are all populated by default values.
 *
 * https://canopen-stack.org/v4.4/start/quickstart/#mandatory-object-entries
 *
 */
#define MANDATORY_IDENTIFICATION_ENTRIES_1000_1014      \
    {                                                   \
        /* Device Type */                               \
        .Key = CO_KEY(0x1000, 0x00, CO_OBJ_____R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) 0x00,                         \
    },                                                  \
    {                                                   \
        /* Error Register */                            \
        .Key = CO_KEY(0x1001, 0x00, CO_OBJ_____R_),     \
        .Type = CO_TUNSIGNED8,                          \
        .Data = (CO_DATA) 0x00,                         \
    },                                                  \
    {                                                   \
        /* COB-ID SYNC-message */                       \
        .Key = CO_KEY(0x1005, 0x00, CO_OBJ_DN__R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) 0x80,                         \
    },                                                  \
    { /* COB-ID EMCY */                                 \
        .Key = CO_KEY(0x1014, 0x00, CO_OBJ__N__R_),     \
        .Type = CO_TEMCY_ID,                            \
        .Data = (CO_DATA) 0x80,                         \
    }

// TODO: Implement a heartbeat consumer

/*!
 * @brief HEARTBEAT PRODUCER OBJECT
 *
 * This macro creates a heartbeat producer with a given interval
 * in milliseconds. This section of the object dictionary lives
 * at the 0x1017 key.
 *
 * @param INTERVAL (integer) the milli-second interval for heartbeats.
 */
#define HEARTBEAT_PRODUCER_1017(INTERVAL)           \
    {                                               \
        .Key = CO_KEY(0x1017, 0x00, CO_OBJ_D___R_), \
        .Type = CO_THB_PROD,                        \
        .Data = (CO_DATA) INTERVAL,                 \
    }

/*!
 * @brief DEVICE IDENTITY OBJECT
 *
 * This macro helps create the object dictionary entry for the 0x1018 key,
 * also known as the Identity Object. This is used to identify the device
 * on the CAN Network.
 *
 * TODO: Currently this macro does not allow for per device product codes, revisions, or serial numbers
 */
#define IDENTITY_OBJECT_1018                            \
    {                                                   \
        /* Object Start Key */                          \
        .Key = CO_KEY(0x1018, 0x00, CO_OBJ_D___R_),     \
        .Type = CO_TUNSIGNED8,                          \
        .Data = (CO_DATA) 0x04,                         \
    },                                                  \
    {                                                   \
        /* Vendor ID */                                 \
        .Key = CO_KEY(0x1018, 0x01, CO_OBJ_____R_),     \
        .Type = CO_TUNSIGNED8,                          \
        .Data = (CO_DATA) 0x00,                         \
    },                                                  \
    {                                                   \
        /* Product Code */                              \
        .Key = CO_KEY(0x1018, 0x02, CO_OBJ_____R_),     \
        .Type = CO_TUNSIGNED8,                          \
        .Data = (CO_DATA) 0x00,                         \
    },                                                  \
    {                                                   \
        /* Revision Number */                           \
        .Key = CO_KEY(0x1018, 0x03, CO_OBJ_____R_),     \
        .Type = CO_TUNSIGNED8,                          \
        .Data = (CO_DATA) 0x00,                         \
    },                                                  \
    { /* Serial Number */                               \
        .Key = CO_KEY(0x1018, 0x04, CO_OBJ_____R_),     \
        .Type = CO_TUNSIGNED8,                          \
        .Data = (CO_DATA) 0x00,                         \
    }

/*!
 * @brief SDO CONFIGURATION OBJECT
 *
 * This macro helps create the mandatory SDO configuration range of the
 * object dictionary. This lives at the key 0x1200 with 2 sub indices.
 * The SDO configuration is the same across all of our boards so this
 * macro has no parameters.
*/
#define SDO_CONFIGURATION_1200                          \
    {                                                   \
        /* Communication Object SDO Server */           \
        .Key = CO_KEY(0x1200, 0x00, CO_OBJ_D___R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) 0x02,                         \
    },                                                  \
    {                                                   \
        /* SDO Server Request COBID */                  \
        .Key = CO_KEY(0x1200, 0x01, CO_OBJ__N__R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) CO_COBID_SDO_REQUEST(),       \
    },                                                  \
    { /* SDO Server Response COBID */                   \
        .Key = CO_KEY(0x1200, 0x02, CO_OBJ__N__R_),     \
        .Type = CO_TUNSIGNED32,                         \
        .Data = (CO_DATA) CO_COBID_SDO_RESPONSE(),      \
    }

/*!
 * @brief RPDO SETTINGS OBJECT
 *
 * This macro creates an RPDO settings object. This macro itself is abstract,
 * allowing it to be used with any RPDO number supported by CANOpen. To make
 * interaction with the entire object dictionary easier you just specify a number
 * starting at 0. This macro also takes in information about the TPDO that is this
 * RPDO receives data from. This is the TPDO number and the NODE ID.
 *
 * @param RPDO_NUMBER (integer) the RPDO number this settings object is for.
 * @param TPDO_NUMBER (integer) the number for the TPDO that this RPDO receives from
 * @param TPDO_NODE_ID (hex) the ID of the CAN Node to receive a TPDO from
 * @param TRANSMISSION_TYPE (hex) the type of transmission to make. You should use RECEIVE_PDO_TRIGGER_SYNC or RECEIVE_PDO_TRIGGER_ASYNC.
*/
#define RECEIVE_PDO_SETTINGS_OBJECT_140X(RPDO_NUMBER, TPDO_NUMBER, TPDO_NODE_ID, TRANSMISSION_TYPE) \
    {                                                                                               \
        /* RPDO #N Settings Object */                                                               \
        .Key = CO_KEY(0x1400 + RPDO_NUMBER, 0x00, CO_OBJ_D___R_),                                   \
        .Type = CO_TUNSIGNED8,                                                                      \
        .Data = (CO_DATA) 0x02,                                                                     \
    },                                                                                              \
    {                                                                                               \
        /* COB-ID used by TPDO  180h + TPDO Node-ID*/                                               \
        .Key = CO_KEY(0x1400 + RPDO_NUMBER, 0x01, CO_OBJ_D___R_),                                   \
        .Type = CO_TPDO_ID,                                                                         \
        .Data = (CO_DATA) CO_COBID_TPDO_DEFAULT(TPDO_NUMBER) + TPDO_NODE_ID,                        \
    },                                                                                              \
    { /* Transmission type */                                                                       \
        .Key = CO_KEY(0x1400 + RPDO_NUMBER, 0x02, CO_OBJ_D___R_),                                   \
        .Type = CO_TPDO_TYPE,                                                                       \
        .Data = (CO_DATA) TRANSMISSION_TYPE,                                                        \
    }

/*!
 * @brief RPDO MAPPING START KEY
 *
 * This is a macro that helps create an RPDO Mapping start key. Because
 * RPDOs can have a dynamic length we are not able to make a single macro
 * that creates the entire dictionary. To remedy this we have this start key
 * which defines the first key in an RPDO Mapping which starts at 0x160x where x
 * is the RPDO Number.
 *
 * This macro takes in two parameters, the RPDO number which is used to create
 * the Object dictionary key as well as the number of sub indices that you are mapping.
 *
 * @param RPDO_NUMBER (integer) the RPDO number that this Mapping Key is associated with.
 * @param NUMBER_OF_SUB_INDICES (integer) the number of sub indices this mapping object will have.
 */
#define RECEIVE_PDO_MAPPING_START_KEY_16XX(RPDO_NUMBER, NUMBER_OF_SUB_INDICES)   \
    {                                                                            \
        .Key = CO_KEY(0x1600 + RPDO_NUMBER, 0x00, CO_OBJ_D___R_),                \
        .Type = CO_TUNSIGNED8,                                                   \
        .Data = (CO_DATA) NUMBER_OF_SUB_INDICES,                                 \
    }

/*!
 * @brief RPDO MAPPING ENTRY
 *
 * This is a macro that allows us to define a mapping entry for an RPDO of number N and
 * an entry of sub index N. The RPDO_NUMBER should be consistent with the previous
 * RPDO_N_MAPPING_START_KEY_160X to ensure that this entry is mapped to the correct Mapping
 * object. You must also keep track of the sub index that this entry is in relation to the
 * previous Start Key.
 *
 * @param RPDO_NUMBER (integer) the RPDO number that this entry is associated with.
 * @param SUB_INDEX (integer) the sub index in the greater RPDO MAPPING Object.
 * @param DATA_SIZE (integer) the size in bits of the data that this entry corresponds to. You should use the PDO_MAPPING_UNSIGNEDX definitions for these sizes.
 */
#define RECEIVE_PDO_MAPPING_ENTRY_16XX(RPDO_NUMBER, SUB_INDEX, DATA_SIZE)             \
    {                                                                                 \
        .Key = CO_KEY(0x1600 + RPDO_NUMBER, SUB_INDEX, CO_OBJ_D___R_),                \
        .Type = CO_TUNSIGNED32,                                                       \
        .Data = (CO_DATA) CO_LINK(0x2100 + RPDO_NUMBER, 0x00 + SUB_INDEX, DATA_SIZE), \
    }

/*!
 * @brief TPDO SETTINGS OBJECT
 *
 * This macro creates an TPDO settings object. This macro itself is abstract,
 * allowing it to be used with any TPDO number supported by CANOpen.
 *
 * @param TPDO_NUMBER (integer) the TPDO number this settings object is for.
 * @param TRANSMISSION_TYPE (hex) the type of transmission to make. You should use TRANSMIT_PDO_TRIGGER_TIMER.
 * @param INHIBIT_TIME (integer) The amount of time (in 100μs increments) that must pass before another TPDO message can be sent.
 * @param INTERVAL (integer) the time trigger (in ms) that the TPDO sends on (0 = disable).
 *
*/
#define TRANSMIT_PDO_SETTINGS_OBJECT_18XX(TPDO_NUMBER, TRANSMISSION_TYPE, INHIBIT_TIME, INTERVAL) \
    {                                                                                             \
        /* TPDO #N Settings Object */                                                             \
        .Key = CO_KEY(0x1800 + TPDO_NUMBER, 0x00, CO_OBJ_D___R_),                                 \
        .Type = CO_TUNSIGNED8,                                                                    \
        .Data = (CO_DATA) 0x05,                                                                   \
    },                                                                                            \
    {                                                                                             \
        /* COB-ID used by TPDO  180h+TPDO Node-ID*/                                               \
        .Key = CO_KEY(0x1800 + TPDO_NUMBER, 0x01, CO_OBJ_DN__R_),                                 \
        .Type = CO_TPDO_ID,                                                                       \
        .Data = (CO_DATA) CO_COBID_TPDO_DEFAULT(TPDO_NUMBER),                                     \
    },                                                                                            \
    {                                                                                             \
        /* Transmission type */                                                                   \
        .Key = CO_KEY(0x1800 + TPDO_NUMBER, 0x02, CO_OBJ_D___R_),                                 \
        .Type = CO_TPDO_TYPE,                                                                     \
        .Data = (CO_DATA) TRANSMISSION_TYPE,                                                      \
    },                                                                                            \
    {                                                                                             \
        /* Inhibit time with LSB 100us (0=disable) */                                             \
        .Key = CO_KEY(0x1800 + TPDO_NUMBER, 0x03, CO_OBJ_D___R_),                                 \
        .Type = CO_TUNSIGNED16,                                                                   \
        .Data = (CO_DATA) INHIBIT_TIME,                                                           \
    },                                                                                            \
    { /* Event timer LSB 1ms (0=disable) */                                                       \
        .Key = CO_KEY(0x1800 + TPDO_NUMBER, 0x05, CO_OBJ_D___R_),                                 \
        .Type = CO_TPDO_EVENT,                                                                    \
        .Data = (CO_DATA) INTERVAL,                                                               \
    }

/*!
 * @brief TPDO MAPPING START KEY
 *
 * This macro helps create a TPDO Mapping start key. Because
 * TPDOs can have a dynamic length we are not able to make a single macro
 * that creates the entire dictionary. To remedy this we have this start key
 * which defines the first key in a TPDO Mapping which starts at 0x1A0x where x
 * is the TPDO Number.
 *
 * This macro takes in two parameters, the TPDO number which is used to create
 * the Object dictionary key as well as the number of sub indices that you are mapping.
 * @param TPDO_NUMBER (integer) the TPDO number that this Mapping Key is associated with.
 * @param NUMBER_OF_SUB_INDICES (integer) the number of sub indices this mapping object will have.
 */
#define TRANSMIT_PDO_MAPPING_START_KEY_1AXX(TPDO_NUMBER, NUMBER_OF_SUB_INDICES)   \
    {                                                                             \
        .Key = CO_KEY(0x1A00 + TPDO_NUMBER, 0x00, CO_OBJ_D___R_),                 \
        .Type = CO_TUNSIGNED8,                                                    \
        .Data = (CO_DATA) NUMBER_OF_SUB_INDICES,                                  \
    }

/*!
 * @brief TPDO MAPPING ENTRY
 *
 * This macro creates a mapping entry for a TPDO mapping object. The TPDO number should
 * relate to the TPDO that this mapping should be packaged with. The sub index is relative
 * to the previous Start Key and should increment by one for each entry. The Data Size should
 * utilize the PDO_MAPPING_UNSIGNEDx data types where x is the size in bits of the data that
 * this mapping relates too.
 *
 * @param RPDO_NUMBER (integer) the TPDO number that this entry is associated with.
 * @param SUB_INDEX (integer) the sub index in the greater TPDO MAPPING Object.
 * @param DATA_SIZE (integer) the size in bits of the data that this entry corresponds to. You should use the PDO_MAPPING_UNSIGNEDX definitions for these sizes.
 */
#define TRANSMIT_PDO_MAPPING_ENTRY_1AXX(TPDO_NUMBER, SUB_INDEX, DATA_SIZE)            \
    {                                                                                 \
        .Key = CO_KEY(0x1A00 + TPDO_NUMBER, SUB_INDEX, CO_OBJ_D___R_),                \
        .Type = CO_TUNSIGNED32,                                                       \
        .Data = (CO_DATA) CO_LINK(0x2100 + TPDO_NUMBER, 0x00 + SUB_INDEX, DATA_SIZE), \
    }

/*!
 * @brief DATA LINK START KEY
 *
 * This is a macro which defines the start of a data linking section of the object dictionary.
 * Because Data Linking can be done for multiple PDOs and should count up from index 2100 based
 * on the PDO number that it is associated with, this macro takes the PDO number associated with the
 * data that will be linked.
 *
 * @param PDO_NUMBER (integer) the PDO number that this data link section is associated with.
 * @param NUMBER_OF_SUB_INDICES (integer) the number of links that this section will include.
 */
#define DATA_LINK_START_KEY_21XX(PDO_NUMBER, NUMBER_OF_SUB_INDICES) \
    {                                                               \
        .Key = CO_KEY(0x2100 + PDO_NUMBER, 0, CO_OBJ_D___R_),       \
        .Type = CO_TUNSIGNED8,                                      \
        .Data = (CO_DATA) NUMBER_OF_SUB_INDICES,                    \
    }

/*!
 * @brief DATA LINK
 *
 * This macro creates a Data Link entry. This allows us to link pointers to data in our
 * application to mapping entries we created previously in a CANOpen Dictionary. This macro
 * takes a significant amount of parameters because it is highly abstract. This was done
 * to allow for both TPDOs and RPDOs of any number to be linked as well as any data type and
 * any data pointers.
 *
 * @param PDO_NUMBER (integer) the PDO number that this data link is associated with
 * @param SUB_INDEX (integer) the sub index in the great Data Link section.
 * @param DATA_TYPE (CO_T...) type of the data that is being linked too. You should use the CANOpen definitions for types here.
 * @param DATA_POINTER (pointer) a pointer to a piece of data that this data link will connect to. Please provide this as a pointer using the &variableName syntax. This macro does not automatically add the &
 */
#define DATA_LINK_21XX(PDO_NUMBER, SUB_INDEX, DATA_TYPE, DATA_POINTER) \
    {                                                                  \
        .Key = CO_KEY(0x2100 + PDO_NUMBER, SUB_INDEX, CO_OBJ____PRW),  \
        .Type = DATA_TYPE,                                             \
        .Data = (CO_DATA) DATA_POINTER,                                \
    }

#endif//EVT_CANOPENMACROS_HPP
