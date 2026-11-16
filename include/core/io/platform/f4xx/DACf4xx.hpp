#ifndef _EVT_DACf4xx_
#define _EVT_DACf4xx_

#include <core/io/DAC.hpp>
#include <core/io/pin.hpp>

namespace core::io {

/**
 * DAC output channel for the STM32F4xx.
 *
 * The pin selects the DAC channel, Timer Trigger, and DMA stream.
 *
 * F446 / F469
 *   PA4 => DAC1, Channel 1, TIM7 Trigger, DMA1 stream 5 (request channel 7)
 *   PA5 => DAC1, Channel 2, TIM7 Trigger, DMA1 stream 6 (request channel 7)
 *
 * Buffered output uses TIM7 (TIM6 is the RTOS tick), so TIM7 must not be used for anything
 * else while buffered output runs. Both channels share TIM7, so they must use the same
 * sample rate.
 */
class DACf4xx : public DACBase {
public:
    /**
     * Set up the pin and DAC channel, starting in static mode at 0V.
     * Should use getDAC() instead of calling this directly.
     *
     * @param[in] pin A pin for which isValidPin() is true
     */
    explicit DACf4xx(Pin pin);

    /**
     * Check whether a pin supports DAC output.
     *
     * @param[in] pin The pin to check
     * @return true if the pin can be used with getDAC()
     */
    static constexpr bool isValidPin(Pin pin) {
        return slotFor(pin) != INVALID_SLOT;
    }

    DACStatus startBuffer(const uint16_t* samples, uint16_t length, uint32_t sampleHz, bool circular = true) override;

    void stopBuffer() override;

    /**
     * Handle a DMA event for this channel. Called from the DMA interrupt, don't call this yourself...
     *
     * @param[in] event The event that occurred
     */
    void handleEvent(DACEvent event);

private:
    /// Returned by slotFor() for pins without a DAC output
    static constexpr uint8_t INVALID_SLOT = 0xFF;

    /**
     * Map a pin to its array index.
     *
     * @param[in] pin The pin to look up
     * @return The slot index, or INVALID_SLOT
     */
    static constexpr uint8_t slotFor(Pin pin) {
        switch (pin) {
        case Pin::PA_4:
            return 0;
        case Pin::PA_5:
            return 1;
        default:
            return INVALID_SLOT;
        }
    }

    void writeRaw(uint16_t value) override;

    uint16_t readRaw() const override;

    /**
     * Configure the channel's trigger source. The channel must be disabled.
     *
     * @param[in] trigger A HAL DAC_TRIGGER_* value
     */
    void configureChannel(uint32_t trigger);

    /// Index into the hardware resource table
    const uint8_t slot;
    /// Whether the channel is in timer-triggered mode and holds a reference on TIM7
    bool triggered = false;
    /// Whether the running buffer repeats
    bool circular = false;
};

} // namespace core::io

#endif // _EVT_DACf4xx_
