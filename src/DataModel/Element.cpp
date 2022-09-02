#include "Element.h"

Element::Element()
{
    this->float_value = 0.0;
    this->int_value = 0;
    this->long_value = 0;
    this->unsigned_int_value = 0;
    this->unsigned_long_value = 0;

    timestamp_tz_t unix = "1970-01-01 00:00:00+09";
    this->timestamp = unix;
}

Element::~Element()
{
}

bool Element::append(float *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    float_value = *add_value;
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(int *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    int_value = *add_value;
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(long *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    long_value = *add_value;
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned int *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    unsigned_int_value = *add_value;
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned long *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    unsigned_long_value = *add_value;
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::pick_value(float *_value)
{
    *_value = float_value;
    return true;
}

bool Element::pick_value(int *_value)
{
    *_value = int_value;
    return true;
}

bool Element::pick_value(long *_value)
{
    *_value = long_value;
    return true;
}

bool Element::pick_value(unsigned int *_value)
{
    *_value = unsigned_int_value;
    return true;
}

bool Element::pick_value(unsigned long *_value)
{
    *_value = unsigned_long_value;
    return true;
}

timestamp_tz_t Element::pick_timestamp()
{
    return timestamp;
}

bool Element::saved_value_is(float value)
{
    return value == float_value;
}

bool Element::saved_value_is(int value)
{
    return value == int_value;
}

bool Element::saved_value_is(long value)
{
    return value == long_value;
}

bool Element::saved_value_is(unsigned int value)
{
    return value == unsigned_int_value;
}

bool Element::saved_value_is(unsigned long value)
{
    return value == unsigned_long_value;
}

timestamp_tz_t Element::saved_timestamp()
{
    return timestamp;
}