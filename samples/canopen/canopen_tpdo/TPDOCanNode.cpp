#include "TPDOCanNode.hpp"

TPDOCanNode::TPDOCanNode() {
    sampleDataA = 0;
    sampleDataB = 1;
}

void TPDOCanNode::setSampleDataA(uint8_t newValue) {
    this->sampleDataA = newValue;
}

uint8_t TPDOCanNode::getSampleDataA() {
    return sampleDataA;
}

void TPDOCanNode::setSampleDataB(uint16_t newValue) {
    this->sampleDataB = newValue;
}

uint16_t TPDOCanNode::getSampleDataB() {
    return sampleDataB;
}

void TPDOCanNode::update() {
    sampleDataA++;
    if (sampleDataA % 50 == 0) {
//        for (uint16_t &output : outData.outputs) {
//            output++;
//        }
    }
}

CO_OBJ_T* TPDOCanNode::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t TPDOCanNode::getNumElements() {
    return sizeof(objectDictionary) / sizeof(objectDictionary[0]);
}

uint8_t TPDOCanNode::getNodeID() {
    return NODE_ID;
}