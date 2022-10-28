#include "TestCanNode.hpp"

TestCanNode::TestCanNode() {
    sampleDataA = 0;
    sampleDataB = 0;
}

void TestCanNode::setSampleDataA(uint8_t newValue) {
    this->sampleDataA = newValue;
}

uint8_t TestCanNode::getSampleDataA() {
    return sampleDataA;
}

void TestCanNode::setSampleDataB(uint16_t newValue) {
    this->sampleDataB = newValue;
}

uint16_t TestCanNode::getSampleDataB() {
    return sampleDataB;
}

CO_OBJ_T* TestCanNode::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t TestCanNode::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}
