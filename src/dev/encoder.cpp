#include <EVT/dev/encoder.hpp>

namespace EVT::core::DEV {

Encoder::Encoder(IO::GPIO& a, IO::GPIO& b, int64_t range, int64_t initialPosition): a(a), b(b), range(range), position(initialPosition){
    //making sure range is positive
    if (range < 0) {
        range *= -1;
    }
    setPosition(position);
    //setting other instance variables
    currentDirection = Static;
    noChangeCounter = 0;
    currentRelPos = readPinValues();
}

int64_t Encoder::getPosition() {
    return position;
}

int8_t Encoder::update() {
    //Reads current pin values
    int8_t newPos = readPinValues();
    //find the change in values;
    int8_t change = calculateChange(newPos);
    //sets the position to its new value
    setPosition(position + change);
    return change;
}

int8_t Encoder::calculateChange(int8_t newRelPos) {
    //Gets the change between the positions and then sets currentRelPos to the newRelPos
    int8_t change = newRelPos - currentRelPos;
    currentRelPos = newRelPos;

    //Basically, if the switch is static, only accepts inputs that are
    // exactly 1 to the left or right
    if (currentDirection == Static) {
        switch(change) {
        case -1:
            setDirection(Left);
            break;
        case 1:
            setDirection(Right);
            break;
        case -3:
            setDirection(Right);
            change = 1;
            break;
        case 3:
            setDirection(Left);
            change = -1;
            break;
        default:
            change = 0;
        }
        return change;
    }
    //Checking if the change is 0, which could either mean the encoder actually moved 0
    //or the direction is STATIC and the change is 2 or -2, in which case we can't determine which direction
    //the encoder was turning in. (that case is handled by the default case in the STATIC switch statement)
    if (change == 0) {
        incrementNoChangeCounter();
        return change;
    } else {
        noChangeCounter = 0;
    }
    //If the change is 3 or -3, that's basically it switching from 3 to 0
    // or 0 to 3, which is really a change of 1 or -1.
    if (change == 3 || change == -3) {
        change /= 3;
    }
    //The only different for non-STATIC directions is that they assume a change of 2
    // is in the direction the encoder is already moving in, so the logic is greatly simplified
    if (currentDirection == Left) {
        if (change < 0) {
            return change;
        } else {
            change *= -1;
            return change;
        }
    }
    if (currentDirection == Right) {
        if (change < 0) {
            change *= -1;
            return change;
        } else {
            return change;
        }
    }
}

void Encoder::setDirection(Direction newDirection) {
        if (newDirection == Static) {
            noChangeCounter = 0;
        }
        currentDirection = newDirection;
}

bool Encoder::incrementNoChangeCounter() {
        noChangeCounter++;
        bool atCap = noChangeCounter >= noChangeCap;
        if (atCap) {
            setDirection(Static);
        }
        return atCap;
}

int8_t Encoder::readPinValues() {
    bool aPos = (bool)a.readPin();
    bool bPos = (bool)b.readPin();
    int8_t newPos = convertPinValuesToPosition(aPos,bPos);
    return newPos;
}

int8_t Encoder::convertPinValuesToPosition(bool a, bool b) {
    if (a == 0 && b == 0) {return 0;}
    if (a == 1 && b == 0) {return 1;}
    if (a == 1 && b == 1) {return 2;}
    if (a == 0 && b == 1) {return 3;}
}

bool Encoder::setPosition(int64_t newPosition) {
    bool capped = false;
    if (newPosition > range) {
            newPosition = range;
            capped = true;
    } else if (newPosition < -1*range) {
            newPosition = -1 * range;
            capped = true;
    }
    position = newPosition;
    return capped;
}

uint8_t Encoder::getNoChangeCounter() {
    return noChangeCounter;
}

int8_t Encoder::getRelativePosition() {
    return currentRelPos;
}

} // namespace EVT::core::DEV