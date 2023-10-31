#include <EVT/dev/encoder.hpp>

namespace EVT::core::DEV {



Encoder::Encoder(IO::GPIO& a, IO::GPIO& b, uint64_t range, uint64_t initialPosition, bool rollOver)
    : a(a), b(b), range(range), position(initialPosition), rollOver(rollOver){
    if (position > range) {
        position = range;
    }
    //setting instance variables
    currentRelPos = readPinValues();
    interruptChange = 0;
    recentInterruptTime = 0;
}


void Encoder::aInterruptHandler(IO::GPIO* pin) {
    if(time::millis() - recentInterruptTime < INTERRUPTCOOLDOWN) {
        return;
    }
    recentInterruptTime = time::millis();
    int8_t change;
    switch(currentRelPos) {
        case 0:
            change = 1;
            currentRelPos = 1;
            break;
        case 1:
            change = -1;
            currentRelPos = 0;
            break;
        case 2:
            change = 1;
            currentRelPos = 3;
            break;
        case 3:
            change = -1;
            currentRelPos = 2;
            break;
    }
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "aInterrupt Called, position: %d",currentRelPos);
    interruptChange += change;
}

void Encoder::bInterruptHandler(IO::GPIO* pin) {
    if(time::millis() - recentInterruptTime < INTERRUPTCOOLDOWN) {
            return;
    }
    recentInterruptTime = time::millis();
    int8_t change;
    switch(currentRelPos) {
    case 0:
            change = -1;
            currentRelPos = 3;
            break;
    case 1:
            change = 1;
            currentRelPos = 2;
            break;
    case 2:
            change = -1;
            currentRelPos = 1;
            break;
    case 3:
            change = 1;
            currentRelPos = 0;
            break;
    }
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "bInterrupt Called, position: %d",currentRelPos);
    interruptChange += change;
}

uint64_t Encoder::getPosition() const {
    return position;
}

void Encoder::update() {
    int64_t change = interruptChange;
    interruptChange = 0;
    changePosition(change);
}

int8_t Encoder::readPinValues() {
    bool aPos = (bool)a.readPin();
    bool bPos = (bool)b.readPin();
    int8_t newPos = convertPinValuesToPosition(aPos,bPos);
    return newPos;
}

int8_t Encoder::convertPinValuesToPosition(bool a, bool b) {
    if (a == 0 && b == 0) {return 0;}
    else if (a == 1 && b == 0) {return 1;}
    else if (a == 1 && b == 1) {return 2;}
    else if (a == 0 && b == 1) {return 3;}
}

bool Encoder::changePosition(int64_t change) {
    change %= (int64_t)range;
    bool hitCap = false;
    int64_t newPosition = (int64_t)position;
    newPosition += change;
    if (newPosition < 0) {
            if (rollOver) {
                position = (int64_t)(range)+newPosition+1;
            } else {
                position = 0;
            }
            hitCap = true;
    } else if (newPosition > (int64_t)range) {
            //Position reached cap
            if (rollOver) {
                position = (uint64_t)newPosition -1 - range;
            } else {
                position = range;
            }
            hitCap = true;
    } else {
            position = (uint64_t)newPosition;
    }
    return hitCap;
}

} // namespace EVT::core::DEV
