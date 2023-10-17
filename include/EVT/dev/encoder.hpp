#ifndef _EVT_ENCODER_
#define _EVT_ENCODER_

#include <EVT/io/GPIO.hpp>

namespace EVT::core::DEV {
class Encoder {
public:
    Encoder(IO::GPIO& a, IO::GPIO& b, int64_t range, int64_t initialPosition);

    int64_t update();

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
    int noChangeCounter;

    static int8_t convertPinValuesToPosition(bool a, bool b);
}; // namespace EVT::core::DEV
} // namespace EVT::core::DEV

#endif