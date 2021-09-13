#include <EVT/io/platform/f3xx/f302x8/Flashf302x8.hpp>

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal_flash.h>

namespace EVT::core::IO {

Flashf302x8::Flashf302x8() {

    // Check to see if the custom
    uint8_t currentPageConfig = *reinterpret_cast<uint8_t*>
        (OB_DATA_ADDRESS_DATA0);
    FLASH_OBProgramInitTypeDef optionProgram;

    HAL_FLASHEx_OBErase();
    
    /* Disable read and write protection for interfacing with flash */
    optionProgram.OptionType = static_cast<uint32_t>
        (OPTIONBYTE_WRP);
    optionProgram.WRPState   = static_cast<uint32_t>
        (OB_WRPSTATE_DISABLE);
    
    HAL_FLASH_Unlock();     // unlock flash interface
    HAL_FLASH_OB_Unlock();  // unlock option bytes

    HAL_FLASHEx_OBProgram(&optionProgram);

    HAL_FLASH_Lock();       // lock up
    HAL_FLASH_OB_Lock();    // lock up

    optionProgram.OptionType = static_cast<uint32_t>
        (OPTIONBYTE_RDP);
    optionProgram.RDPLevel   = static_cast<uint32_t>
        (OB_RDP_LEVEL_0);

    HAL_FLASH_Unlock();     // unlock flash interface
    HAL_FLASH_OB_Unlock();  // unlock option bytes

    HAL_FLASHEx_OBProgram(&optionProgram);

    HAL_FLASH_Lock();       // lock up
    HAL_FLASH_OB_Lock();    // lock up


    optionProgram.OptionType  = static_cast<uint32_t>(OPTIONBYTE_DATA);
    optionProgram.DATAAddress = static_cast<uint32_t>(OB_DATA_ADDRESS_DATA0);
    optionProgram.DATAData    = CUSTOM_PAGE_CONFIG;
    HAL_FLASH_Unlock();     // unlock flash interface
    HAL_FLASH_OB_Unlock();  // unlock option bytes

    HAL_FLASHEx_OBProgram(&optionProgram);

    HAL_FLASH_Lock();       // lock up
    HAL_FLASH_OB_Lock();    // lock up

    HAL_FLASH_OB_Launch();  // apply changes

    FLASH_EraseInitTypeDef flashErase;
    flashErase.TypeErase   = FLASH_TYPEERASE_PAGES;  // how to perform erase operation
    flashErase.PageAddress = CUSTOM_PAGE_CONFIG;           // address of starting page to erase
    flashErase.NbPages     = 1;            // how many pages to erase

    HAL_FLASH_Unlock(); // unlock the flash interface
    uint32_t error;
    HAL_FLASHEx_Erase(&flashErase, &error);  // perform flash erase
    HAL_FLASH_Lock();   // lock up

}

uint8_t Flashf302x8::readByte(uint32_t address) {
    return *reinterpret_cast<uint8_t*>(USER_FLASH_START - address); 
}

uint16_t Flashf302x8::readHalfWord(uint32_t address) {
    return *reinterpret_cast<uint16_t*>(USER_FLASH_START - address);
}

uint32_t Flashf302x8::readWord(uint32_t address) {
    return *reinterpret_cast<uint32_t*>(USER_FLASH_START - address);
}

uint64_t Flashf302x8::readDoubleWord(uint32_t address) {
    return *reinterpret_cast<uint64_t*>(USER_FLASH_START - address);
}

void Flashf302x8::writeByte(uint32_t address, uint8_t data) {
    HAL_FLASH_Unlock();
    
    // The STM32f302x8 doesn't support byte level writing, so a read/write
    // logic is required
    uint16_t fullData = readHalfWord(address);

    // Either put the 8 bit data in upper half or lower half of the data
    if(address % 16 == 0) {
        fullData = (fullData & 0xFF) | data;
    }
    else {
        fullData = (fullData & ~(0xFF)) | (static_cast<uint16_t>(data) << 8);
    }

    // Write out the data
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, fullData);

    HAL_FLASH_Lock();
}

void Flashf302x8::writeHalfWord(uint32_t address, uint16_t data) {
    HAL_FLASH_Unlock();

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data);

    HAL_FLASH_Lock();
}

void Flashf302x8::writeWord(uint32_t address, uint32_t data) {
    HAL_FLASH_Unlock();

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);

    HAL_FLASH_Lock();
}

void Flashf302x8::writeDoubleWord(uint32_t address, uint64_t data) {
    HAL_FLASH_Unlock();

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data);

    HAL_FLASH_Lock();
}
    

}  // EVT::core::IO
