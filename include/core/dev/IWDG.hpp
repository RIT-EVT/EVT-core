#ifndef _EVT_IWDG_
#define _EVT_IWDG_

namespace core::dev {

/**
 * The IWDG is the independent watchdog, which is used to detect software failures.
 * This class represents features common to watchdog timers.
 */
class IWDG {
public:
    /**
     * Initializes and activates the watchdog timer.
     */
    virtual void init() = 0;

    /**
     * Refreshes the watchdog timer.
     */
    virtual void refresh() = 0;
};

} // namespace core::dev

#endif
