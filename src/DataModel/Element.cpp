#include "Element.h"

Element::Element()
{
    timestamp_tz_t unix = "1970-01-01 00:00:00+09";
    this->timestamp = unix;
}

Element::~Element()
{
}

bool Element::append(float *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    nucleus.putFloat(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(int *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    nucleus.putInt(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(long *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    nucleus.putLong(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned int *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    nucleus.putUnsignedInt(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::append(unsigned long *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    nucleus.putUnsignedLong(*add_value);
    // DONE : Test code is in "/examples"
    return true;
}

bool Element::pick_value(float *_value)
{
    *_value = nucleus.getFloat();
    return true;
}

bool Element::pick_value(int *_value)
{
    *_value = nucleus.getInt();
    return true;
}

bool Element::pick_value(long *_value)
{
    *_value = nucleus.getLong();
    return true;
}

bool Element::pick_value(unsigned int *_value)
{
    *_value = nucleus.getUnsignedInt();
    return true;
}

bool Element::pick_value(unsigned long *_value)
{
    *_value = nucleus.getUnsignedLong();
    return true;
}

timestamp_tz_t Element::pick_timestamp()
{
    return timestamp;
}

bool Element::saved_value_is(float value)
{
    return value == nucleus.getFloat();
}

bool Element::saved_value_is(int value)
{
    return value == nucleus.getInt();
}

bool Element::saved_value_is(long value)
{
    return value == nucleus.getLong();
}

bool Element::saved_value_is(unsigned int value)
{
    return value == nucleus.getUnsignedInt();
}

bool Element::saved_value_is(unsigned long value)
{
    return value == nucleus.getUnsignedLong();
}

bool Element::saved_timestamp_is(timestamp_tz_t timestamp)
{
    return timestamp == this->timestamp;
}