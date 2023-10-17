#include <EVT/dev/encoder.hpp>

namespace EVT::core::DEV {

Encoder::Encoder(IO::GPIO& a, IO::GPIO& b, int64_t range, int64_t initialPosition): a(a), b(b), range(range), position(initialPosition){
    if (range < 0) {
        range *= -1;
    }
    if (position > range) {
        position = range;
    } else if (position < -1*range) {
        position = -1*range;
    }
}

int64_t Encoder::update() {
    bool aPos = (bool)a.readPin();
    bool bPos = (bool)b.readPin();
    int8_t newPos = convertPinValuesToPosition(aPos,bPos);

}

int8_t Encoder::convertPinValuesToPosition(bool a, bool b) {
    if (a == 0 && b == 0) {return 0;}
    if (a == 1 && b == 0) {return 1;}
    if (a == 1 && b == 1) {return 2;}
    if (a == 0 && b == 1) {return 3;}
}

} // namespace EVT::core::DEV