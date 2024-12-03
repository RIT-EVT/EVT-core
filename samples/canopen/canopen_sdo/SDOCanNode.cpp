#include "SDOCanNode.hpp"

SDOCanNode::SDOCanNode() {
    sampleDataA = 6;
    sampleDataB = 0;
}

void SDOCanNode::SDO_Transfer(CO_NODE &node) {
    /* Increment the first element of sampleDataArray by 1. */
    sampleDataArray[0]++;
    /* Set the second element of sampleDataArray to twice the new value of the first element. */
    sampleDataArray[1] = sampleDataArray[0] * 2;

    /* Initiate an SDO transfer using the specified node and sampleDataArray,
     * targeting object dictionary entry 0x2100:02. */
    CO_ERR err = core::io::SDOTransfer(node, sampleDataArray, 2, CO_DEV(0x2100,0x02));

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
    /* Initiate an SDO receive operation, reading data into sampleDataArray
     * from object dictionary entry 0x2100:01. */
    CO_ERR err = core::io::SDOReceive(node, sampleDataArray, 1, CO_DEV(0x2100,0x01));

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

uint8_t SDOCanNode::getSampleDataA() {
    return sampleDataA;
}

uint16_t SDOCanNode::getSampleDataB() {
    return sampleDataB;
}

void SDOCanNode::update() {
    sampleDataA++;
    if (sampleDataA % 20 == 0) {
        sampleDataB *= 3;
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