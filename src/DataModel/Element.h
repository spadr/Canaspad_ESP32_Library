#ifndef Element_h
#define Element_h

#include "..\Type.h"

class Element
{
public:
    float float_value;
    int int_value;
    long long_value;
    unsigned int unsigned_int_value;
    unsigned long unsigned_long_value;

    timestamp_tz_t timestamp;
    Element();
    ~Element();

    bool append(float *add_value, timestamp_tz_t timestamp_add);
    bool append(int *add_value, timestamp_tz_t timestamp_add);
    bool append(long *add_value, timestamp_tz_t timestamp_add);
    bool append(unsigned int *add_value, timestamp_tz_t timestamp_add);
    bool append(unsigned long *add_value, timestamp_tz_t timestamp_add);

    bool saved_value_is(float value);
    bool saved_value_is(int value);
    bool saved_value_is(long value);
    bool saved_value_is(unsigned int value);
    bool saved_value_is(unsigned long value);
    timestamp_tz_t saved_timestamp();
};

#endif
