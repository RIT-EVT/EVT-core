#include <EVT/dev/encoder.hpp>
#include <EVT/utils/time.hpp>

namespace EVT::core::DEV {

Encoder::Encoder(IO::GPIO& a, IO::GPIO& b): a(a), b(b) {
    relPos = 0;
    direction = Direction::Right;
}

void Encoder::update() {
    bool aPos = (bool)a.readPin();
    bool bPos = (bool)b.readPin();
}

uint8_t convertPinValuesToPosition(bool a, bool b) {
    if (a == 0 && b == 0) {return 1;}
    if (a == 1 && b == 0) {return 2;}
    if (a == 1 && b == 1) {return 3;}
    if (a == 0 && b == 1) {return 4;}
}

} // namespace EVT::core::DEV