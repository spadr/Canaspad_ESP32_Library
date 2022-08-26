#include "Element.h"

Element::Element()
{
    this->float_value = 0.0;
    this->int_value = 0;
    this->long_value = 0;
    this->unsigned_int_value = 0;
    this->unsigned_long_value = 0;

    timestamp_tz_t unix = "1970-01-01 00:00:00";
    this->timestamp = unix;
}

Element::~Element()
{
}

bool Element::append(float *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    float value = *add_value;
    Serial.println("Element::append " + String(value) + " " + String(timestamp_add));//TODO : Test
    return true;
}

bool Element::append(int *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    int value = *add_value;
    Serial.println("Element::append " + String(value));//TODO : Test
    return true;
}

bool Element::append(long *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    long value = *add_value;
    Serial.println("Element::append " + String(value));//TODO : Test
    return true;
}

bool Element::append(unsigned int *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    unsigned int value = *add_value;
    Serial.println("Element::append " + String(value));//TODO : Test
    return true;
}

bool Element::append(unsigned long *add_value, timestamp_tz_t timestamp_add)
{
    timestamp = timestamp_add;
    unsigned long value = *add_value;
    Serial.println("Element::append " + String(value));//TODO : Test
    return true;
}

void Element::get_value(float *value_p)
{
    *value_p = float_value;
}

void Element::get_value(int *value_p)
{
    *value_p = int_value;
}

void Element::get_value(long *value_p)
{
    *value_p = long_value;
}

void Element::get_value(unsigned int *value_p)
{
    *value_p = unsigned_int_value;
}

void Element::get_value(unsigned long *value_p)
{
    *value_p = unsigned_long_value;
}

timestamp_tz_t Element::get_timestamp()
{
    return timestamp;
}
