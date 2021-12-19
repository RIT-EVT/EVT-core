#ifndef _EVT_IWDG_
#define _EVT_IWDG_

namespace EVT::core::DEV {

/**
     * The IWDG is the independent watchdog, which is used to detect software failures.
     * This class represents features common to watchdog timers.
     */
class IWDG {
public:
    /**
         * Refreshes the watchdog timer.
         */
    virtual void refresh() = 0;
};

}// namespace EVT::core::DEV

#endif