#ifndef Element_h
#define Element_h

//#include <vector>
//#include <iostream>
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

    void get_value(float *value_p);
    void get_value(int *value_p);
    void get_value(long *value_p);
    void get_value(unsigned int *value_p);
    void get_value(unsigned long *value_p);

    timestamp_tz_t get_timestamp();
};

#endif
