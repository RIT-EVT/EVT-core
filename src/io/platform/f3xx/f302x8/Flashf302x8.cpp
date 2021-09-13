#include <EVT/io/platform/f3xx/f302x8/Flashf302x8.hpp>

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal_flash.h>

namespace EVT::core::IO {

Flashf302x8::Flashf302x8() {

    // Check to see if the custom
    uint8_t currentPageConfig = *reinterpret_cast<uint8_t*>
        (OB_DATA_ADDRESS_DATA0);
    if(currentPageConfig != CUSTOM_PAGE_CONFIG) {
        FLASH_OBProgramInitTypeDef optionProgram;

        HAL_FLASHEx_OBErase();
        
        /* Disable read and write protection for interfacing with flash */
        optionProgram.OptionType = static_cast<uint32_t>
            (OB_WRPSTATE_ENABLE);
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

    }
}

uint8_t Flashf302x8::readByte(uint32_t address) {

}

uint16_t Flashf302x8::readHalfWord(uint32_t address) {

}

uint32_t Flashf302x8::readWord(uint32_t address) {

}

uint64_t Flashf302x8::readDoubleWord(uint32_t address) {

}

void Flashf302x8::writeByte(uint32_t address, uint8_t data) {

}

void Flashf302x8::writeHalfWord(uint32_t address, uint16_t data) {

}

void Flashf302x8::writeWord(uint32_t address, uint32_t data) {

}

void Flashf302x8::writeDoubleWord(uint32_t address, uint64_t data) {

}
    

}  // EVT::core::IO
