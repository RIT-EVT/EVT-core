#ifndef _EVT_ENCODER_
#define _EVT_ENCODER_

#include <EVT/io/GPIO.hpp>

namespace EVT::core::DEV {
class Encoder {
public:
    Encoder(IO::GPIO& a, IO::GPIO& b);

    void update();

    enum Direction {
        Left = 0u,
        Right = 1u
    };



private:
    IO::GPIO& a;
    IO::GPIO& b;
    uint8_t relPos;
    Direction direction;

}; // namespace EVT::core::DEV
} // namespace EVT::core::DEV

#endif