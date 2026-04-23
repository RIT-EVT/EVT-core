#include "SDOCanNode.hpp"

#include <cstdio>

#include <core/utils/log.hpp>

namespace log = core::log;

SDOCanNode::SDOCanNode(CO_NODE& canNode) : node(canNode), transferBuff(0) {}

void SDOCanNode::transferData(io::csdo_callback_t callback, void* context) {
    // Get the value to be transferred
    transferBuff = static_cast<uint8_t*>(context);

    /*
     * Initiates an SDO transfer for the specified node using the provided
     * transfer buffer array. Targets the object dictionary entry at index 0x2100,
     * sub-index 0x02. Registers and executes the SDOTransferCallback function upon completion.
     */
    CO_ERR err = io::SDOTransfer(node, transferBuff, 2, CO_DEV(0x2100, 0x02), callback, transferBuff);

    /* Check if the SDO transfer was successfully started. */
    if (err == CO_ERR_NONE) {
        /* Transfer is started successfully */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "SDOTransfer Sent Request");
    } else {
        /* Unable to start the SDO transfer */
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "SDOTransfer Request Error");
    }
}

void SDOCanNode::receiveData(io::csdo_callback_t callback, void* context) {
    /*
     * Initiates an SDO receive operation for the specified node, reading data into
     * the provided receive buffer array. Targets the object dictionary entry at
     * index 0x2100, sub-index 0x01. Registers and executes the SDOReceiveCallback function upon completion.
     */
    CO_ERR err = io::SDOReceive(node, (uint8_t*) context, 1, CO_DEV(0x2100, 0x01), callback, context);

    /* Check if the SDO receive operation was successfully started. */
    if (err == CO_ERR_NONE) {
        /* Receive is started successfully */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "SDOReceive Sent Request");
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