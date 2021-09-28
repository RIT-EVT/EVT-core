#include <EVT/io/CANopenNVM.hpp>

#include <stdint.h>

#define MAX_SIZE 64

namespace {
    uint8_t testerStorage[MAX_SIZE];
}

/**
 * Initialize the NVM driver, does nothing
 */
static void nvmInit(void) {

}

/**
 * Read from the temporary buffer
 */
static uint32_t nvmRead(uint32_t start, uint8_t* buffer, uint32_t size) {
    uint32_t bytesRead = 0;

    for(unsigned int i = 0; i < size && i + start < MAX_SIZE; i++) {
        buffer[i] = testerStorage[i + start];
        bytesRead++;
    }

    return bytesRead;
}

/**
 * Write to the temporary buffer
 */
static uint32_t nvmWrite(uint32_t start, uint8_t* buffer, uint32_t size) {
    uint32_t bytesWrote = 0;

    for(unsigned int i = 0; i < size && i + start < MAX_SIZE; i++) {
        testerStorage[i + start] = buffer[i];
        bytesWrote++;
    }
    return bytesWrote;
}

namespace EVT::core::IO {
    
    void getCANopenNVMDriver(CO_IF_NVM_DRV* nvmDriver) {
        nvmDriver->Init = nvmInit;
        nvmDriver->Read = nvmRead;
        nvmDriver->Write = nvmWrite;
    }

}
