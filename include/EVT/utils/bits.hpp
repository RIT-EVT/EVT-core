/**
 * Provides a series of utilities for operating on the binary representation
 * of a number. These utilites are commonly used operations that may be needed.
 */

#ifndef _BITS_H_
#define _BITS_H_

#include <cstdint>

// 16 bit operations
/**
 * Get the high byte from the number.
 *
 * @param data The number to get the high byte from
 * @return The high order byte
 */
constexpr static inline uint8_t getHighByte(uint16_t data)
{
    return static_cast<uint8_t>((data & 0xFF00) >> 8);
}

/**
 * Get the low byte from the number.
 *
 * @param data The number to get the low byte from
 * @return The low order byte
 */
constexpr static inline uint8_t getLowByte(uint16_t data)
{
    return static_cast<uint8_t>(data & 0x00FF);
}

// 32 bit operations
/**
 * Get the highest byte from the number
 *
 * @param data The number to get the highest byte from
 * @return The highest order byte
 */
constexpr static inline uint8_t getHighestByte(uint32_t data)
{
    return static_cast<uint8_t>((data & 0xFF000000) >> 24);
}

/**
 * Get the upper middle byte from the number.
 *
 * @param data The number to get the upper middle byte from
 * @return The upper middle order byte
 */
constexpr static inline uint8_t getMiddleHighByte(uint32_t data)
{
    return static_cast<uint8_t>((data & 0x00FF0000) >> 16);
}

/**
 * Get the lower middle byte from the number.
 *
 * @param data The number to get the lower middle byte from.
 * @return The lower middle order byte
 */
constexpr static inline uint8_t getMiddleLowByte(uint32_t data)
{
    return static_cast<uint8_t>((data & 0x0000FF00) >> 8);
}

/**
 * The the lowest byte from the number.
 *
 * @param data The number to get the lowest byte from.
 * @return The lowest order byte
 */
constexpr static inline uint8_t getLowestByte(uint32_t data)
{
    return static_cast<uint8_t>((data & 0x000000FF));
}

#endif
