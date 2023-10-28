#include "RPDOCanNode.hpp"

RPDOCanNode::RPDOCanNode() {
    sampleDataA = 0;
    sampleDataB = 0;
}

void RPDOCanNode::setSampleDataA(uint8_t newValue) {
    this->sampleDataA = newValue;
}

uint8_t RPDOCanNode::getSampleDataA() {
    return sampleDataA;
}

void RPDOCanNode::setSampleDataB(uint16_t newValue) {
    this->sampleDataB = newValue;
}

uint16_t RPDOCanNode::getSampleDataB() {
    return sampleDataB;
}

CO_OBJ_T* RPDOCanNode::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t RPDOCanNode::getNumElements() {
    return OBJECT_DICTIONARY_SIZE + 1;
}
