#include "TestCanNode.hpp"

TestCanNode::TestCanNode() {
    sampleData = 0;
}

void TestCanNode::setSampleData(uint8_t newValue) {
    this->sampleData = newValue;
}

uint8_t TestCanNode::getSampleData() {
    return sampleData;
}

CO_OBJ_T* TestCanNode::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t TestCanNode::getNumElements() {
    return OBJECT_DIRECTIONARY_SIZE;
}
