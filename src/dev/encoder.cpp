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
    if (change == 0) {
        incrementNoChangeCounter();
        return change;
    } else {
        noChangeCounter = 0;
    }
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
        if (noChangeCounter >= noChangeCap) {
            setDirection(Static);
            return true;
        }
        return false;
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
    if (newPosition > range) {
            newPosition = range;
            return true;
    } else if (newPosition < range) {
            newPosition = -1 * range;
            return true;
    }
    return false;
}

uint8_t Encoder::getNoChangeCounter() {
    return noChangeCounter;
}

} // namespace EVT::core::DEV