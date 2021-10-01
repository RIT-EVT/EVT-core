#ifndef EVT_EEPROM_HPP
#define EVT_EEPROM_HPP

#include <EVT/io/manager.hpp>


namespace EVT::core::IO {
/**
 * EEPROMs are a type of small non-volatile storage devices. This class
 * represents features common across all EEPROMs.
 */
    class EEPROM {

    public:
        /**
         * Read 8 bits of data from the given address.
         *
         * @param[in] address The address to read from
         * @return The 8 bits of data at the address
         */
        virtual uint8_t readByte(uint32_t address) = 0;

        /**
         * Read 16 bits of data from the given address.
         *
         * @param[in] address The address to read from
         * @return The 16 bits of data at the address
         */
        virtual uint16_t readHalfWord(uint32_t address) = 0;

        /**
         * Read 32 bits of data from the given address.
         *
         * @param[in] address The address to read from
         * @return The 32 bits of data at the address
         */
        virtual uint32_t readWord(uint32_t address) = 0;

        /**
         * Read 8 bits of data from each of the given addresses.
         *
         * @param[in] addresses The addresses to read from
         * @return A list of 8 bits of data stored at the given addresses
         */
        virtual uint8_t * readBytes(uint8_t addresses[]) = 0;

        /**
         * Read 16 bits of data from each of the given addresses.
         *
         * @param[in] addresses The addresses to read from
         * @return A list of 16 bits of data stored at the given addresses
         */
        virtual uint16_t * readHalfWords(uint8_t addresses[]) = 0;

        /**
         * Read 32 bits of data from each of the given addresses.
         *
         * @param[in] addresses The addresses to read from
         * @return A list of 32 bits of data stored at the given addresses
         */
        virtual uint32_t * readWords(uint8_t addresses[]) = 0;

        /**
         * Write 8 bits of data to the given address.
         *
         * @param[in] address The address to write to
         * @param[in] data The data to write out
         */
        virtual void writeByte(uint32_t address, uint8_t data) = 0;

        /**
         * Write 16 bits of data to the given address.
         *
         * @param[in] address The address to write to
         * @param[in] data The data to write out
         */
        virtual void writeHalfWord(uint32_t address, uint16_t data) = 0;

        /**
         * Write 32 bits of data to the given address.
         *
         * @param[in] address The address to write to
         * @param[in] data The data to write out
         */
        virtual void writeWord(uint32_t address, uint32_t data) = 0;

        /**
         * Write 8 bits of data to each of the given addresses.
         *
         * @param addresses The addresses to write to
         * @param[in] dataArr The data to write out
         */
        virtual void writeBytes(uint8_t addresses[], uint8_t dataArr[]) = 0;

        /**
         * Write 16 bits of data to each of the given addresses.
         *
         * @param addresses The addresses to write to
         * @param[in] dataArr The data to write out
         */
        virtual void writeHalfWords(uint8_t addresses[], uint16_t dataArr[]) = 0;

        /**
         * Write 32 bits of data to each of the given addresses.
         *
         * @param addresses The addresses to write to
         * @param[in] dataArr The data to write out
         */
        virtual void writeWords(uint8_t addresses[], uint32_t dataArr[]) = 0;
    };
}


#endif //EVT_EEPROM_HPP
