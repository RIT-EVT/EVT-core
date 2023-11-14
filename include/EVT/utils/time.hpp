/**
 * The functions defined in here are used for time based operations. All
 * operations are platform independent.
 *
 */

#ifndef _EVT_TIME_
#define _EVT_TIME_

#include <cstdint>

namespace EVT::core::time {
/**
 * Function to have the program hold for a set amount of time before
 * continuing.
 *
 * @param ms The number of milliseconds to wait for
 */
void wait(uint32_t ms);

/**
 * Get the milliseconds since system startup
 *
 * @return Time in milliseconds
 */
uint32_t millis();

/**
 * Struct representing a timestamp.
 */
struct TimeStamp {
    /**
     * The current year
     */
    uint16_t year;
    /**
     * The current month (1-12)
     */
    uint8_t month;
    /**
     * The current day (1-31)
     */
    uint8_t day;
    /**
     * The hour in a 24 hour timeframe
     */
    uint8_t hour;
    /**
     * The minute (0-59)
     */
    uint8_t minute;
    /**
     * The second (0-59)
     */
    uint8_t second;
};
}// namespace EVT::core::time
#endif
