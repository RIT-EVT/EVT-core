#include "SDOCanNode.hpp"

SDOCanNode::SDOCanNode() {
    sampleDataA = 0;
    sampleDataB = 0;
}

void SDOCanNode::SDOTransfer(CO_NODE node) {

    CO_CSDO *csdo = COCSdoFind(&(node), 0);
    CO_ERR   err  = COCSdoRequestDownload(csdo, CO_DEV(0x00,0x01),
                              &sampleDataA, 8,
                              AppCSdoFinishCb, 1000);

    if (err == CO_ERR_NONE) {

        /* Transfer is started successfully */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Value transferred: %lu", sampleDataA);

        /* Note: don't use the 'readValue' until transfer is finished! */

    } else {
        /* Unable to start the SDO transfer */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Download Error");

    }
}

void SDOCanNode::SDOReceive(CO_NODE node) {
    CO_CSDO *csdo;
    CO_ERR   err;

    csdo = COCSdoFind(&(node), 0);
    err = COCSdoRequestUpload(csdo, CO_DEV(0x00,0x02),
                              &sampleDataB, 8,
                              AppCSdoFinishCb, 1000);

    if (err == CO_ERR_NONE) {

        /* Transfer is started successfully */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Value : %lu", sampleDataB);

        /* Note: don't use the 'readValue' until transfer is finished! */

    } else {
        /* Unable to start the SDO transfer */
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Upload Error");

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

/* The application specific SDO transfer finalization callback */
void AppCSdoFinishCb(CO_CSDO *csdo, uint16_t index, uint8_t sub, uint32_t code)
{
    if (code == 0) {
        /* read data is available in 'readValue' */

    }
    else {
        /* a timeout or abort is detected during SDO transfer  */
    }

}