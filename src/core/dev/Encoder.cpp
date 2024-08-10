#include <core/dev/Encoder.hpp>

namespace EVT::core::DEV {

Encoder::Encoder(IO::GPIO& a, IO::GPIO& b, uint32_t range, uint32_t initialPosition, bool rollOver)
    : a(a), b(b), range(range), position(initialPosition), rollOver(rollOver) {
    if (position > range) {
        position = range;
    }
    //setting instance variables
    currentRelPos = readPinValues();
    a.registerIRQ(IO::GPIO::TriggerEdge::RISING_FALLING, aInterruptHandlerWrapper, this);
    b.registerIRQ(IO::GPIO::TriggerEdge::RISING_FALLING, bInterruptHandlerWrapper, this);
}

void Encoder::aInterruptHandlerWrapper(IO::GPIO* pin, void* instance) {
    auto* e = (Encoder*) instance;
    e->aInterruptHandler();
}

void Encoder::bInterruptHandlerWrapper(IO::GPIO* pin, void* instance) {
    auto* e = (Encoder*) instance;
    e->bInterruptHandler();
}

void Encoder::aInterruptHandler() {
    if ((time::millis() - lastAInterruptTime) < INTERRUPTCOOLDOWN) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "aInterrupt Skipped, reason: on cooldown");
        return;
    }
    int8_t change;
    switch (currentRelPos) {
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
    interruptChange += change;
    lastAInterruptTime = time::millis();
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "aInterrupt Called, "
                                                  "\n\r\tCalculated Position: %d"
                                                  "\n\r\tActual Position: %d"
                                                  "\n\r\tChange: %d",
                    currentRelPos, readPinValues(), change);
}

void Encoder::bInterruptHandler() {
    if ((time::millis() - lastBInterruptTime) < INTERRUPTCOOLDOWN) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "bInterrupt Skipped, reason: on cooldown");
        return;
    }
    int8_t change;
    switch (currentRelPos) {
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
    interruptChange += change;
    lastBInterruptTime = time::millis();
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "bInterrupt Called, "
                                                  "\n\r\tCalculated Position: %d"
                                                  "\n\r\tActual Position: %d"
                                                  "\n\r\tChange: %d",
                    currentRelPos, readPinValues(), change);
}

uint32_t Encoder::getPosition() {
    int64_t change = interruptChange;
    interruptChange = 0;
    changePosition(change);
    //in case of triggers desyncing the relative position
    currentRelPos = readPinValues();
    return position;
}

void Encoder::setRangeAndPosition(uint32_t newRange, uint32_t newPosition) {
    range = newRange;
    if (newPosition <= range) {
        position = newPosition;
    } else {
        position = 0;
    }
}

int8_t Encoder::readPinValues() {
    bool aPos = (bool) a.readPin();
    bool bPos = (bool) b.readPin();
    //calculating and returning the position
    if (aPos == 0 && bPos == 0) {
        return 0;
    } else if (aPos == 1 && bPos == 0) {
        return 1;
    } else if (aPos == 1 && bPos == 1) {
        return 2;
    } else if (aPos == 0 && bPos == 1) {
        return 3;
    }
}

bool Encoder::changePosition(int64_t change) {
    change %= (int64_t) (range + 1);
    bool hitCap = false;
    int64_t newPosition = (int64_t) position;
    newPosition += change;
    if (newPosition < 0) {
        if (rollOver) {
            position = (uint32_t) ((range) + (uint32_t) newPosition + 1);
        } else {
            position = 0;
        }
        hitCap = true;
    } else if (newPosition > (int64_t) range) {
        //Position reached cap
        if (rollOver) {
            position = (uint32_t) newPosition - 1 - range;
        } else {
            position = range;
        }
        hitCap = true;
    } else {
        position = (uint32_t) newPosition;
    }
    return hitCap;
}

}// namespace EVT::core::DEV
