#include "SDOCanNode.hpp"
#include <core/io/CANopen.hpp>
#include <cstdio>

SDOCanNode::SDOCanNode() {
    sampleDataA          = 0;
    sampleDataB          = 0;
    transferBuffArray[0] = 0;
    transferBuffArray[1] = 0;
}

void SDOCanNode::SdoReceiveCallback(CO_CSDO* csdo, uint16_t index, uint8_t sub, uint32_t code) {
    char messageString[50];
    if (code == 0) {
        /* read data is available in 'readValue' */
        snprintf(&messageString[0], 25, "Value received %x, %x\r\n", csdo->Tfer.Buf[0], csdo->Tfer.Buf[1]);
    } else {
        /* a timeout or abort is detected during SDO transfer  */
        snprintf(&messageString[0], 25, "SDO receive callback don goofed 0x%x\r\n", code);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "SDO Receive Operation: \r\n\t%s\r\n", messageString);
}

void SDOCanNode::SdoTransferCallback(CO_CSDO* csdo, uint16_t index, uint8_t sub, uint32_t code) {
    char messageString[50];
    if (code == 0) {
        /* read data is available in 'readValue' */
        snprintf(&messageString[0], 25, "Value transferred %x, %x\r\n", csdo->Tfer.Buf[0], csdo->Tfer.Buf[1]);
    } else {
        /* a timeout or abort is detected during SDO transfer  */
        snprintf(&messageString[0], 25, "SDO transfer callback don goofed 0x%x\r\n", code);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "SDO Transfer Operation: \r\n\t%s\r\n", messageString);
}

void SDOCanNode::transferData(CO_NODE& node) {
    /* Increment the first element of transferBuffArray by 1. */
    transferBuffArray[0]++;
    /* Set the second element of transferBuffArray to twice the new value of the first element. */
    transferBuffArray[1] = transferBuffArray[0] * 2;

    /*
     * Initiates an SDO transfer for the specified node using the provided
     * transfer buffer array. Targets the object dictionary entry at index 0x2100,
     * sub-index 0x02. Registers and executes the SDOTransferCallback function upon completion.
     */
    CO_ERR err = core::io::SDOTransfer(node, transferBuffArray, 2, CO_DEV(0x2100, 0x02), SdoTransferCallback);

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

void SDOCanNode::receiveData(CO_NODE& node) {
    static uint8_t receiveBuffArray[1];

    /*
     * Initiates an SDO receive operation for the specified node, reading data into
     * the provided receive buffer array. Targets the object dictionary entry at
     * index 0x2100, sub-index 0x01. Registers and executes the SDOReceiveCallback function upon completion.
     */
    CO_ERR err = core::io::SDOReceive(node, receiveBuffArray, 1, CO_DEV(0x2100, 0x01), SdoReceiveCallback);

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