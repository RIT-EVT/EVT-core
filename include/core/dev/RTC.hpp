#ifndef _EVT_RTC_
#define _EVT_RTC_

#include <core/utils/time.hpp>

namespace core::DEV {

/**
 * The RTC is the real time clock interface. This class represents features
 * common acrross real time clocks.
 */
class RTC {
public:
    /**
     * Get the current time as determined by the real time clock.
     *
     * @param[out] time The time struct to populate
     */
    virtual void getTime(core::time::TimeStamp& time) = 0;

    /**
     * Get the timestamp since epoch in seconds
     *
     * @return The time since epoch as determined by the RTC
     */
    virtual uint32_t getTime() = 0;

    /**
     * Set the time of the real time clock.
     *
     * @param[in] time The time to set the RTC to use.
     */
    virtual void setTime(core::time::TimeStamp& time) = 0;
};

} // namespace core::DEV

#endif
