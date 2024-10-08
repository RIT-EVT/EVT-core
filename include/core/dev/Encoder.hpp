#ifndef _EVT_ENCODER_
#define _EVT_ENCODER_

#include <core/io/GPIO.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace time = core::time;

namespace core::dev {

/**
 * This class represents a CTS Series 288 Rotary encoder with a 2-bit Binary resolution.
 * The static wrappers for the interrupt handles must be created in the main method
 * see samples/encoder/main.cpp for example implementation.
 * update() must be called within the main loop to update encoder position.
 */
class Encoder {
public:
    /**
     * Constructs an Encoder instance.
     *
     * @param[in] a a pin of the encoder
     * @param[in] b b pin of the encoder
     * @param[in] range range of the encoder positions
     * @param[in] initialPosition initial position the encoder is in
     */
    Encoder(io::GPIO& a, io::GPIO& b, uint32_t range, uint32_t initialPosition, bool rollOver);

    /**
     * Returns the current absolute position
     *
     * @return the current position of the encoder, between 0 and range, inclusive
     */
    uint32_t getPosition();

    /**
     * Sets the range and position of the encoder
     *
     * @param newRange the new range for the encoder positions
     * @param newPosition a new position for the encoder (defaults to 0)
     */
    void setRangeAndPosition(uint32_t newRange, uint32_t newPosition = 0);

    /**
     * Static Wrapper for aInterruptHandler()
     *
     * @param[in] pin pin that caused the interrupt
     * @param[in] instance instance of an encoder object
     */
    static void aInterruptHandlerWrapper(io::GPIO* pin, void* instance);

    /**
     * Static Wrapper for bInterruptHandler()
     *
     * @param[in] pin pin that caused the interrupt
     * @param[in] instance instance of an encoder object
     */
    static void bInterruptHandlerWrapper(io::GPIO* pin, void* instance);

private:
    /** GPIO pin a */
    io::GPIO& a;
    /** GPIO pin b */
    io::GPIO& b;

    /** position can be in [0, range] */
    uint32_t range;
    /** the current absolute position of the encoder */
    uint32_t position;
    /** if the position rolls over when it reaches range or 0 */
    bool rollOver;
    /** counter for how many updates have occurred since the encoder was moved */
    int8_t currentRelPos;

    // INTERRUPT RELATED VARIABLES
    /** the change since the last update was called, updated in the interrupt handler */
    int64_t interruptChange = 0;
    /** the most recent time in milliseconds since the last interrupt on pin A */
    uint32_t lastAInterruptTime = 0;
    /** the most recent time in milliseconds since the last interrupt on pin B*/
    uint32_t lastBInterruptTime = 0;
    /** how soon after an interrupt another can happen, in milliseconds */
    const static uint32_t INTERRUPTCOOLDOWN = 100;

    /**
     * reads pin values and converts them to the relative position
     *
     * @return relative position of the encoder
     */
    int8_t readPinValues();

    /**
     * changes position by change, capping or rolling over depending on rollOver
     *
     * @param[in] change the the position should be changed by
     * @return if position tried to go outside the range
     */
    bool changePosition(int64_t change);

    /**
     * Handles the interrupt generated by pin a. If two interrupts occur too soon after one another the second is
     * ignored.
     */
    void aInterruptHandler();

    /**
     * Handles the interrupt generated by pin b. If two interrupts occur too soon after one another the second is
     * ignored.
     */
    void bInterruptHandler();
};

} // namespace core::dev

#endif
