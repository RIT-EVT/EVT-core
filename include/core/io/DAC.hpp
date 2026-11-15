#ifndef _EVT_DAC_
#define _EVT_DAC_

#include <cstdint>

namespace core::io {

/**
 * A single DAC (digital to analog) output channel.
 *
 * The DAC can be static or buffered:
 *  - Static: setValue() / setVoltage() change the output immediately and is fixed until set again.
 *  - Buffered: startBuffer() streams samples from memory at a fixed sample rate.
 *    A hardware timer triggers each conversion and DMA feeds the samples.
 *    An optional callback fires at the half-way point and the end of the buffer, which allows
 *    double-buffering: refill the first half on HALF_COMPLETE and the second half on COMPLETE.
 *
 * Any static write stops buffered output.
 */
class DACBase {
public:
    /** Result of starting buffered output */
    enum class DACStatus {
        OK    = 0,
        BUSY  = 1,
        ERROR = 2,
    };

    /** Buffered output events, delivered to the callback from interrupt context */
    enum class DACEvent {
        HALF_COMPLETE = 0,
        COMPLETE      = 1,
        ERROR         = 2,
    };

    /**
     * Callback for buffered output events. Runs in ISR, so keep it short.
     *
     * @param[in] event The event that occurred
     * @param[in] priv The pointer given to setCallback()
     */
    using DACCallback = void (*)(DACEvent event, void* priv);

    /// Largest raw value (12-bit resolution)
    static constexpr uint16_t MAX_RAW = 4095;
    /// Fastest supported buffered sample rate in Hz, from datasheet [assumes small steps at max speed]
    static constexpr uint32_t MAX_SAMPLE_RATE = 1'000'000;

    /**
     * Set the output to a raw value. Values above MAX_RAW are clamped.
     * Stops buffered output if it is running.
     *
     * @param[in] value The raw value (0 to MAX_RAW)
     */
    void setValue(uint16_t value);

    /**
     * Get the raw value currently being output, in either mode.
     *
     * @return The raw output value
     */
    uint16_t getValue() const;

    /**
     * Set the output to a voltage. Values outside 0 to Vref are clamped.
     * Stops buffered output if it is running.
     *
     * @param[in] voltage The output voltage in volts
     */
    void setVoltage(float voltage);

    /**
     * Get the voltage currently being output, in either mode.
     *
     * @return The output voltage in volts
     */
    float getVoltage() const;

    /**
     * Set the reference voltage used to convert between volts and raw values.
     *
     * @param[in] vref The reference voltage in volts (default 3.3)
     */
    void setVref(float vref);

    /**
     * Get the reference voltage used to convert between volts and raw values.
     *
     * @return The reference voltage in volts
     */
    float getVref() const;

    /**
     * Start streaming raw samples (0 to MAX_RAW) to the output. If buffered output is
     * already running, it is restarted with the new buffer.
     *
     * The buffer is read by DMA while output runs, so it must stay valid until
     * stopBuffer() is called, or, for non-circular output, until COMPLETE is delivered.
     *
     * Channels that share a trigger timer must use the same sample rate (see the
     * platform class for which timer each channel uses).
     *
     * @param[in] samples The samples to output
     * @param[in] length Number of samples in the buffer
     * @param[in] sampleHz Samples per second, 1 to MAX_SAMPLE_RATE
     * @param[in] circular true to repeat the buffer until stopped, false to play it once
     * @return OK on success, BUSY if the shared trigger timer is running at another
     *         rate, ERROR on invalid arguments or a hardware failure
     */
    virtual DACStatus startBuffer(const uint16_t* samples, uint16_t length, uint32_t sampleHz, bool circular = true) = 0;

    /**
     * Stop buffered output. The output holds the last sample that was played.
     */
    virtual void stopBuffer() = 0;

    /**
     * Check whether buffered output is running. Non-circular output stops running
     * once the whole buffer has been played.
     *
     * @return true if buffered output is running
     */
    bool isBufferRunning() const;

    /**
     * Set the callback for buffered output events.
     * Call it before startBuffer(), otherwise it can cause a race condition...
     *
     * @param[in] callback The function to call, or nullptr to disable
     * @param[in] priv A pointer passed back to the callback
     */
    void setCallback(DACCallback callback, void* priv = nullptr);

protected:
    /**
     * Write a raw value to the output in static mode, leaving buffered mode if needed.
     *
     * @param[in] value The raw value, already clamped to MAX_RAW
     */
    virtual void writeRaw(uint16_t value) = 0;

    /**
     * Read the raw value currently being output.
     *
     * @return The raw output value
     */
    virtual uint16_t readRaw() const = 0;

    /**
     * Deliver a buffered output event to the user callback. Called from interrupt context.
     *
     * @param[in] event The event that occurred
     */
    void notify(DACEvent event);

    /// Whether buffered output is running, updated from interrupt context
    volatile bool bufferRunning = false;

private:
    /// Reference voltage used for voltage conversions
    float vref = 3.3f;
    /// User callback for buffered output events
    DACCallback callback = nullptr;
    /// User context passed to the callback
    void* callbackPriv = nullptr;
};

} // namespace core::io

#endif // _EVT_DAC_
