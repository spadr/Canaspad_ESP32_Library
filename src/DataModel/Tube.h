#ifndef Tube_h
#define Tube_h

//#include <ArduinoJson.h>
#include "Element.h"

class Tube : public Element
{
private:
    uuid_t token;
    String channel;
    String name;
    bool element_empty;
    bool token_empty;
    bool timestamp_empty;

    float *float_value_ptr;
    int *int_value_ptr;
    long *long_value_ptr;
    unsigned int *unsigned_int_value_ptr;
    unsigned long *unsigned_long_value_ptr;

public:
    // DONE : Create a scheme for keeping data types
    Tube(float *sensing_value);
    Tube(int *sensing_value);
    Tube(long *sensing_value);
    Tube(unsigned int *sensing_value);
    Tube(unsigned long *sensing_value);
    ~Tube();

    // DONE : DataModel saves and analyzes the values
    bool begin(String channel, String name, uuid_t token);
    bool add(timestamp_tz_t timestamp);
    void value(float *pick_value);
    void value(int *pick_value);
    void value(long *pick_value);
    void value(unsigned int *pick_value);
    void value(unsigned long *pick_value);
    timestamp_tz_t timestamp();
};

#endif
