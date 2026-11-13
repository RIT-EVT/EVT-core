#ifndef _EVT_PWMf3xx_
#define _EVT_PWMf3xx_

#include <core/dev/TimerTypes.hpp>
#include <core/dev/platform/f3xx/Timerf3xx.hpp>
#include <core/io/PWM.hpp>

namespace core::io {

/**
 * @brief A PWM instance for F3xx family STM32s
 *
 * A subclass of both @ref PWM and @ref TimerF3xx. Since PWM on the F3 requires its own instance of a timer, a subclass
 * is used instead of an internal timer variable. This provides more control over the timer, and is more accurate to
 * what PWM is on the F3 (just a timer instance that flips a GPIO pin).
 */
class PWMf3xx : public PWM, public dev::TimerF3xx {
public:
    /** @inheritDoc */
    explicit PWMf3xx(Pin pin);

    /** @inheritDoc */
    void setDutyCycle(uint32_t dutyCycle) override;

    /**
     * @inheritDoc PWM::setPeriod
     * This function will hide the definition from the TimerF3xx. So you can safely ignore the warning.
     */
    void setPeriod(uint32_t period);

    /** @inheritDoc */
    uint32_t getDutyCycle() override;

    /** @inheritDoc */
    uint32_t getPeriod() override;

private:
    static constexpr dev::TimerConfiguration_t defaultConfig = {dev::TimerCounterMode::UP,
                                                                dev::TimerClockDivision::DIVISION_1,
                                                                dev::TimerAutoReloadPreload::BUFFER,
                                                                dev::TimerClockSource::INTERNAL,
                                                                dev::TimerMasterModeSelection::RESET,
                                                                dev::TimerMasterSlaveMode::DISABLE};

    /**
     * The channel that the timer will run on for this PWM instance
     * */
    uint32_t halTIMChannelID{};
};

} // namespace core::io

#endif
