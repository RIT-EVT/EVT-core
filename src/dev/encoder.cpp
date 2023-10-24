#include <EVT/dev/encoder.hpp>

namespace EVT::core::DEV {

Encoder::Encoder(IO::GPIO& a, IO::GPIO& b, uint64_t range, uint64_t initialPosition, bool rollOver)
    : a(a), b(b), range(range), position(initialPosition), rollOver(rollOver){
    if (position > range) {
        position = range;
    }
    //setting other instance variables
    currentDirection = IDLE;
    noChangeCounter = 0;
    currentRelPos = readPinValues();
}

uint64_t Encoder::getPosition() {
    return position;
}

int8_t Encoder::update() {
    //Read current pin values
    int8_t newPos = readPinValues();
    //find the change in values;
    int8_t change = calculateChange(newPos);
    //set the position to its new value
    changePosition(change);
    return change;
}

int8_t Encoder::calculateChange(int8_t newRelPos) {
    //Get the change between the positions and then sets currentRelPos to the newRelPos
    int8_t change = newRelPos - currentRelPos;
    currentRelPos = newRelPos;

    // Basically, if the switch is static, only accept inputs that are
    // exactly 1 to the left or right
    if (currentDirection == IDLE) {
        switch(change) {
        case -1:
            setDirection(LEFT);
            break;
        case 1:
            setDirection(RIGHT);
            break;
        case -3:
            setDirection(RIGHT);
            change = 1;
            break;
        case 3:
            setDirection(LEFT);
            change = -1;
            break;
        default:
            change = 0;
        }
        return change;
    }
    //Checking if the change is 0, which could either mean the encoder actually moved 0
    //or the direction is IDLE and the change is 2 or -2, in which case we can't determine which direction
    //the encoder was turning in.
    if (change == 0) {
        incrementNoChangeCounter();
        return change;
    } else {
        noChangeCounter = 0;
    }
    // If the change is 3 or -3, that's basically it switching from 3 to 0
    // or 0 to 3, which is really a change of 1 or -1.
    if (change == 3 || change == -3) {
        change /= 3;
        change *= -1;
    }
    //The only difference for non-IDLE directions is that they assume a change of 2
    // is in the direction the encoder is already moving in, so the logic is greatly simplified
    if (currentDirection == LEFT) {
        if (change < 0) {
            return change;
        } else if (change == 1) {
            setDirection(RIGHT);
            return change; //Turned one to the right, probably
        } else {
            change *= -1;
            return change;
        }
    }
    if (currentDirection == RIGHT) {
        if (change == -1) {
            setDirection(LEFT);
            return change; //Turned one to the left
        } else if (change < 0) {
            change *= -1;
            return change;
        } else {
            return change;
        }
    }
}

void Encoder::setDirection(Direction newDirection) {
        if (newDirection == IDLE) {
            noChangeCounter = 0;
        }
        currentDirection = newDirection;
}

bool Encoder::incrementNoChangeCounter() {
        noChangeCounter++;
        bool atCap = noChangeCounter >= noChangeCap;
        if (atCap) {
            setDirection(IDLE);
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

bool Encoder::changePosition(int8_t change) {
    //I HAVE NO IDEA IF THIS METHOD WORKS
    bool hitCap = false;
    int64_t newPosition = (int64_t)position;
    newPosition += change;
    if (newPosition > range) {
            //Position reached cap
            if (rollOver) {
                newPosition = newPosition - range;
            } else {
                newPosition = range;
            }
            hitCap = true;
    } else if (newPosition < 0) {
            if (rollOver) {
                newPosition = range;
            } else {
                newPosition = 0;
            }
            hitCap = true;
    }
    position = (uint64_t) newPosition;
    return hitCap;
}

uint8_t Encoder::getNoChangeCounter() {
    return noChangeCounter;
}

} // namespace EVT::core::DEV
