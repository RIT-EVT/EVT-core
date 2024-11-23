#include <core/rtos/Initializable.hpp>

namespace core::rtos {

void Initializable::getName(char* destination, size_t size) {
    if (size > INITIALIZABLE_NAME_MAX_LENGTH) {
        size = INITIALIZABLE_NAME_MAX_LENGTH;
    }
    strncpy(destination, this->name, size);
    destination[size - 1] = '\0';
}

Initializable::Initializable(char* name) {
    strncpy(this->name, name, INITIALIZABLE_NAME_MAX_LENGTH);
    this->name[INITIALIZABLE_NAME_MAX_LENGTH - 1] = '\0';
}

} // namespace core::rtos
