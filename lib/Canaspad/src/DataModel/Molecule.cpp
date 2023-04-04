#include "Molecule.h"

Molecule::Molecule() {
    String unix = "1970-01-01 00:00:00+09";
    this->timestamp = unix;
}

Molecule::~Molecule() {}

bool Molecule::append(uint8_t** add_value, size_t* size, String timestamp_add) {
    this->value = *add_value;
    this->size = *size;
    timestamp = timestamp_add;
    return true;
}

bool Molecule::remove(uint8_t** _value, size_t* _size) {
    *_value = this->value;
    this->value = nullptr;
    *_size = this->size;
    this->size = 0;
    return true;
}

String Molecule::savedTimestamp() { return timestamp; }

bool Molecule::savedTimestampIs(String timestamp) { return timestamp == this->timestamp; }