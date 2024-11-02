#include <core/dev/Encoder.hpp>

namespace core::dev {

Encoder::Encoder(io::GPIO& a, io::GPIO& b, uint32_t range, uint32_t initialPosition, bool rollOver)
    : a(a), b(b), range(range), position(initialPosition), rollOver(rollOver) {
    if (position > range) {
        position = range;
    }
    // setting instance variables
    currentRelPos = readPinValues();
    a.registerIRQ(io::GPIO::TriggerEdge::RISING_FALLING, aInterruptHandlerWrapper, this);
    b.registerIRQ(io::GPIO::TriggerEdge::RISING_FALLING, bInterruptHandlerWrapper, this);
}

void Encoder::aInterruptHandlerWrapper(io::GPIO* pin, void* instance) {
    auto* e = (Encoder*) instance;
    e->aInterruptHandler();
}

void Encoder::bInterruptHandlerWrapper(io::GPIO* pin, void* instance) {
    auto* e = (Encoder*) instance;
    e->bInterruptHandler();
}

void Encoder::aInterruptHandler() {
    if ((time::millis() - lastAInterruptTime) < INTERRUPTCOOLDOWN) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "aInterrupt Skipped, reason: on cooldown");
        return;
    }
    int8_t change = 0;
    switch (currentRelPos) {
    case 0:
        change        = 1;
        currentRelPos = 1;
        break;
    case 1:
        change        = -1;
        currentRelPos = 0;
        break;
    case 2:
        change        = 1;
        currentRelPos = 3;
        break;
    case 3:
        change        = -1;
        currentRelPos = 2;
        break;
    }
    interruptChange += change;
    lastAInterruptTime = time::millis();
}

void Encoder::bInterruptHandler() {
    if ((time::millis() - lastBInterruptTime) < INTERRUPTCOOLDOWN) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "bInterrupt Skipped, reason: on cooldown");
        return;
    }
    int8_t change = 0;
    switch (currentRelPos) {
    case 0:
        change        = -1;
        currentRelPos = 3;
        break;
    case 1:
        change        = 1;
        currentRelPos = 2;
        break;
    case 2:
        change        = -1;
        currentRelPos = 1;
        break;
    case 3:
        change        = 1;
        currentRelPos = 0;
        break;
    }
    interruptChange += change;
    lastBInterruptTime = time::millis();
}

uint32_t Encoder::getPosition() {
    int64_t change  = interruptChange;
    interruptChange = 0;
    changePosition(change);
    // in case of triggers desyncing the relative position
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
    // we are storing these as uints so we can combine them easier.
    uint8_t aPos = a.readPin() == io::GPIO::State::HIGH;
    uint8_t bPos = b.readPin() == io::GPIO::State::HIGH;
    // Calculating and returning the position.
    // Position is in graycode, not binary.
    uint8_t comb = (bPos << 1) | aPos;
    switch (comb & 0x03) {
    case 0b00:
        return 0;
    case 0b01:
        return 1;
    case 0b11:
        return 2;
    case 0b10:
        return 3;
    }
    // This will never happen, the switch statement covers all possible values of comb
    return 0;
}

bool Encoder::changePosition(int64_t change) {
    change %= (int64_t) (range + 1);
    bool hitCap         = false;
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
        // Position reached cap
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

} // namespace core::dev
