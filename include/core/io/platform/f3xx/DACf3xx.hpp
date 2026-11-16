#ifndef _EVT_DACf3xx_
#define _EVT_DACf3xx_

#include <core/io/DAC.hpp>
#include <core/io/pin.hpp>

namespace core::io {

/**
 * DAC output channel for the STM32F3xx.
 *
 * The pin selects the DAC peripheral, channel, Timer Trigger, and DMA channel.
 *
 *   F302x8
 *   PA4 => DAC1, Channel 1, TIM6 Trigger, DMA1 channel 3
 *
 *   F334x8
 *   PA4 => DAC1, Channel 1, TIM6 Trigger, DMA1 channel 3
 *   PA5 => DAC1, Channel 2, TIM6 Trigger, DMA1 channel 4
 *   PA6 => DAC2, Channel 1, TIM7 Trigger, DMA1 channel 5
 *
 * Buffered output uses the trigger timer listed above, so that timer must not be used
 * for anything else while buffered output runs. Channels that share a timer must use
 * the same sample rate.
 */
class DACf3xx : public DACBase {
public:
    /**
     * Set up the pin and DAC channel, starting in static mode at 0V.
     * Should use getDAC() instead of calling this directly.
     *
     * @param[in] pin A pin for which isValidPin() is true
     */
    explicit DACf3xx(Pin pin);

    /**
     * Check whether a pin supports DAC output
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
        case Pin::PA_4: // PA_4 is shared for both F302 and F334
            return 0;
#ifdef STM32F334x8
        case Pin::PA_5:
            return 1;
        case Pin::PA_6:
            return 2;
#endif
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
    /// Whether the channel is in timer-triggered mode and holds a reference on its timer
    bool triggered = false;
    /// Whether the running buffer repeats
    bool circular = false;
};

} // namespace core::io

#endif // _EVT_DACf3xx_
