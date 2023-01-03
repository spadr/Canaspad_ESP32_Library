#include "Element.h"

Element::Element() {
    String unix = "1970-01-01 00:00:00+09";
    this->timestamp = unix;

    this->_nucleus_ptr = std::unique_ptr<Nucleus>(new Nucleus());
    this->nucleus_ptr = _nucleus_ptr.get();
}

Element::~Element() {
    if (this->_nucleus_ptr) {
        _nucleus_ptr.reset(nullptr);
    }
}

bool Element::append(float* add_value, String timestamp_add) {
    timestamp = timestamp_add;
    nucleus_ptr->putFloat(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(int* add_value, String timestamp_add) {
    timestamp = timestamp_add;
    nucleus_ptr->putInt(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(long* add_value, String timestamp_add) {
    timestamp = timestamp_add;
    nucleus_ptr->putLong(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned int* add_value, String timestamp_add) {
    timestamp = timestamp_add;
    nucleus_ptr->putUnsignedInt(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned long* add_value, String timestamp_add) {
    timestamp = timestamp_add;
    nucleus_ptr->putUnsignedLong(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::savedValue(float* value) {
    *value = nucleus_ptr->getFloat();
    return true;
}

bool Element::savedValue(int* value) {
    *value = nucleus_ptr->getInt();
    return true;
}

bool Element::savedValue(long* value) {
    *value = nucleus_ptr->getLong();
    return true;
}

bool Element::savedValue(unsigned int* value) {
    *value = nucleus_ptr->getUnsignedInt();
    return true;
}

bool Element::savedValue(unsigned long* value) {
    *value = nucleus_ptr->getUnsignedLong();
    return true;
}

String Element::savedTimestamp() { return timestamp; }

bool Element::savedValueIs(float value) { return value == nucleus_ptr->getFloat(); }

bool Element::savedValueIs(int value) { return value == nucleus_ptr->getInt(); }

bool Element::savedValueIs(long value) { return value == nucleus_ptr->getLong(); }

bool Element::savedValueIs(unsigned int value) { return value == nucleus_ptr->getUnsignedInt(); }

bool Element::savedValueIs(unsigned long value) { return value == nucleus_ptr->getUnsignedLong(); }

bool Element::savedTimestampIs(String timestamp) { return timestamp == this->timestamp; }