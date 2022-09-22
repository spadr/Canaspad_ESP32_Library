#include "Element.h"

Element::Element() {
    timestamp_tz_t unix = "1970-01-01 00:00:00+09";
    this->timestamp = unix;
}

Element::~Element() {}

bool Element::append(float* add_value, timestamp_tz_t timestamp_add) {
    timestamp = timestamp_add;
    nucleus.putFloat(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(int* add_value, timestamp_tz_t timestamp_add) {
    timestamp = timestamp_add;
    nucleus.putInt(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(long* add_value, timestamp_tz_t timestamp_add) {
    timestamp = timestamp_add;
    nucleus.putLong(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned int* add_value, timestamp_tz_t timestamp_add) {
    timestamp = timestamp_add;
    nucleus.putUnsignedInt(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned long* add_value, timestamp_tz_t timestamp_add) {
    timestamp = timestamp_add;
    nucleus.putUnsignedLong(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::savedValue(float* _value) {
    *_value = nucleus.getFloat();
    return true;
}

bool Element::savedValue(int* _value) {
    *_value = nucleus.getInt();
    return true;
}

bool Element::savedValue(long* _value) {
    *_value = nucleus.getLong();
    return true;
}

bool Element::savedValue(unsigned int* _value) {
    *_value = nucleus.getUnsignedInt();
    return true;
}

bool Element::savedValue(unsigned long* _value) {
    *_value = nucleus.getUnsignedLong();
    return true;
}

timestamp_tz_t Element::savedTimestamp() { return timestamp; }

bool Element::savedValueIs(float value) { return value == nucleus.getFloat(); }

bool Element::savedValueIs(int value) { return value == nucleus.getInt(); }

bool Element::savedValueIs(long value) { return value == nucleus.getLong(); }

bool Element::savedValueIs(unsigned int value) { return value == nucleus.getUnsignedInt(); }

bool Element::savedValueIs(unsigned long value) { return value == nucleus.getUnsignedLong(); }

bool Element::savedTimestampIs(timestamp_tz_t timestamp) { return timestamp == this->timestamp; }