#include "SDOCanNode.hpp"
#include <core/io/CANopen.hpp>

SDOCanNode::SDOCanNode() {
    sampleDataA = 0;
    sampleDataB = 0;
    transferBuffArray[0] = 0;
    transferBuffArray[1] = 0;
}

/**
 * The application-specific callback function for finalizing an SDO transfer/receive operation.
 * @param csdo[in] is the client-SDO object.
 * @param index[in] is the object dictionary index.
 * @param sub[in] is the object dictionary subindex.
 * @param code[in] indicates the completion status of the operation (0 for success, error code otherwise).
 */
void AppCSdoCallback(CO_CSDO *csdo, uint16_t index, uint8_t sub, uint32_t code) {
    if (code == 0) {
        /* read data is available in 'readValue' */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Value transferred %x, %x\r\n", csdo->Tfer.Buf[0], csdo->Tfer.Buf[1]);
    }
    else {
        /* a timeout or abort is detected during SDO transfer  */
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "SDO callback don goofed 0x%x\r\n", code);
    }
}

void SDOCanNode::SDO_Transfer(CO_NODE &node) {
    /* Increment the first element of transferBuffArray by 1. */
    transferBuffArray[0]++;
    /* Set the second element of transferBuffArray to twice the new value of the first element. */
    transferBuffArray[1] = transferBuffArray[0] * 2;

    /* Initiate an SDO transfer using the specified node and transferBuffArray,
     * targeting object dictionary entry 0x2100:02. */
    CO_ERR err = core::io::SDOTransfer(node, transferBuffArray, 2, CO_DEV(0x2100,0x02));

    /* Check if the SDO transfer was successfully started. */
    if (err == CO_ERR_NONE) {
        /* Transfer is started successfully */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "SDOTransfer Sent Request");

        /* Note: don't use the 'readValue' until transfer is finished! */
    } else {
        /* Unable to start the SDO transfer */
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "SDOTransfer Request Error");
    }
}

void SDOCanNode::SDO_Receive(CO_NODE &node) {
    static uint8_t receiveBuffArray[1];

    /* Initiate an SDO receive operation, reading data into sampleDataA address
     * from object dictionary entry 0x2100:01. */
    CO_ERR err = core::io::SDOReceive(node, receiveBuffArray, 1, CO_DEV(0x2100,0x01), AppCSdoCallback() );

    /* Check if the SDO receive operation was successfully started. */
    if (err == CO_ERR_NONE) {
        /* Transfer is started successfully */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "SDOReceive Sent Request");

        /* Note: don't use the 'readValue' until transfer is finished! */
    } else {
        /* Unable to start the SDO transfer */
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "SDOReceive Request Error");
    }
}

CO_OBJ_T* SDOCanNode::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t SDOCanNode::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t SDOCanNode::getNodeID() {
    return NODE_ID;
}