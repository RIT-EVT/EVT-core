#include "TPDOCanNode.hpp"

TPDOCanNode::TPDOCanNode() {
    sampleDataA = 0;
    sampleDataB = 1;
}

void TPDOCanNode::setSampleDataA(uint8_t newValue) { this->sampleDataA = newValue; }

uint8_t TPDOCanNode::getSampleDataA() { return sampleDataA; }

void TPDOCanNode::setSampleDataB(uint16_t newValue) { this->sampleDataB = newValue; }

uint16_t TPDOCanNode::getSampleDataB() { return sampleDataB; }

void TPDOCanNode::update() {
    sampleDataA++;
    if (sampleDataA % 20 == 0) {
        sampleDataB *= 3;
    }
}

CO_OBJ_T* TPDOCanNode::getObjectDictionary() { return &objectDictionary[0]; }

uint8_t TPDOCanNode::getNumElements() { return OBJECT_DICTIONARY_SIZE; }

uint8_t TPDOCanNode::getNodeID() { return NODE_ID; }
