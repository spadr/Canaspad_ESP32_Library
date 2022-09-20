#ifndef Element_h
#define Element_h

#include "../Type.h"
#include "Nucleus.h"

class Element
{
private:
    Nucleus nucleus;
    timestamp_tz_t timestamp;

public:
    Element();
    ~Element();

    bool append(float *add_value, timestamp_tz_t timestamp_add);
    bool append(int *add_value, timestamp_tz_t timestamp_add);
    bool append(long *add_value, timestamp_tz_t timestamp_add);
    bool append(unsigned int *add_value, timestamp_tz_t timestamp_add);
    bool append(unsigned long *add_value, timestamp_tz_t timestamp_add);

    bool pick_value(float *_value);
    bool pick_value(int *_value);
    bool pick_value(long *_value);
    bool pick_value(unsigned int *_value);
    bool pick_value(unsigned long *_value);
    timestamp_tz_t pick_timestamp();

    bool saved_value_is(float value);
    bool saved_value_is(int value);
    bool saved_value_is(long value);
    bool saved_value_is(unsigned int value);
    bool saved_value_is(unsigned long value);
    bool saved_timestamp_is(timestamp_tz_t timestamp);
};

#endif
