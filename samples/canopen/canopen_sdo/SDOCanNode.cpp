#include "SDOCanNode.hpp"
#include <core/io/CANopen.hpp>
#include <core/utils/log.hpp>
#include <cstdio>

namespace log = core::log;

SDOCanNode::SDOCanNode(CO_NODE& canNode) : node(canNode), sampleDataA(0), sampleDataB(0), transferBuffArray{0, 0} {}

void SDOCanNode::transferData() {
    /* Increment the first element of transferBuffArray by 1. */
    transferBuffArray[0]++;
    /* Set the second element of transferBuffArray to twice the new value of the first element. */
    transferBuffArray[1] = transferBuffArray[0] * 2;

    /*
     * Initiates an SDO transfer for the specified node using the provided
     * transfer buffer array. Targets the object dictionary entry at index 0x2100,
     * sub-index 0x02. Registers and executes the SDOTransferCallback function upon completion.
     */
    CO_ERR err = core::io::SDOTransfer(node, transferBuffArray, 2, CO_DEV(0x2100, 0x02));

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

void SDOCanNode::receiveData() {
    static uint8_t receiveBuffArray[1];

    /*
     * Initiates an SDO receive operation for the specified node, reading data into
     * the provided receive buffer array. Targets the object dictionary entry at
     * index 0x2100, sub-index 0x01. Registers and executes the SDOReceiveCallback function upon completion.
     */
    CO_ERR err = core::io::SDOReceive(node, receiveBuffArray, 1, CO_DEV(0x2100, 0x01));

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