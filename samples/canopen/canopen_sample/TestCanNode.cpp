#include "TestCanNode.hpp"

RPDOCanNode::RPDOCanNode() {
    sampleData = 0;
}

void RPDOCanNode::setSampleData(uint8_t newValue) {
    this->sampleData = newValue;
}

uint8_t RPDOCanNode::getSampleData() {
    return sampleData;
}

CO_OBJ_T* RPDOCanNode::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t RPDOCanNode::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}
