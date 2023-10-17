#ifndef _EVT_ENCODER_
#define _EVT_ENCODER_

#include <EVT/io/GPIO.hpp>

namespace EVT::core::DEV {
class Encoder {
public:
    /**
     * Constructs an Encoder instance. Call update() within the main loop to update encoder values.
     * @param a a pin of the encoder
     * @param b b pin of the encoder
     * @param range range of the encoder positions (should be positive)
     * @param initialPosition initial position the encoder is in
     */
    Encoder(IO::GPIO& a, IO::GPIO& b, int64_t range, int64_t initialPosition);

    /**
     * Reads and updates the encoder rotation and the absolute position.
     * @return what position was changed by, should be in this range: [-3,3]
     */
    int8_t update();

    /**
     * returns the current absolute position
     * @return the current position of the encoder, between -range and range, inclusive
     */
    int64_t getPosition();

    /**
     * An Enum that represents the direction the encoder is being spun in.
     * The direction should reset to Static after a couple of reads with no changes.
     */
    enum Direction {
        Left = -1,
        Static = 0,
        Right = 1
    };



private:
    IO::GPIO& a;
    IO::GPIO& b;
    int64_t range;
    int64_t position;
    Direction currentDirection;
    uint8_t noChangeCounter;
    int8_t currentRelPos;
    const static int8_t noChangeCap = 2;

    /**
     * helper method to convert binary pin values to the relative rotation of the encoder, in the range [0,3]
     * @param a value of the a pin
     * @param b value of the b pin
     * @return relative position of the encoder
     */
    static int8_t convertPinValuesToPosition(bool a, bool b);

    /**
     * reads pin values and converts them to the relative position
     * @return relative position of the encoder
     */
    int8_t readPinValues();

    /**
     * sets direction, setting noChangeCounter to 0 if the direction is set to Static
     * @param newDirection value direction will be set to
     */
    void setDirection(Direction newDirection);

    /**
     * increments the noChangeCounter, setting the direction to Static if noChangeCounter is > noChangeCap
     * @return whether or not the direction was set to Static
     */
    bool incrementNoChangeCounter();

    /**
     * parses the raw change value and determines what the change should actually be based on the direction
     * the encoder was already traveling in. Also handles changing directions away from Static, and indirectly to Static
     * if encoder has been idle long enough
     * @param change the raw change value, should be newPos - currentRelPos
     * @return the actual change value
     */
    int8_t parseChange(int8_t change);

    /**
     * sets position to newPosition, capping it if the new value exceeds the range.
     * @param newPosition the new value for position
     * @return if position was capped
     */
    bool setPosition(int64_t newPosition);

}; // namespace EVT::core::DEV
} // namespace EVT::core::DEV

#endif