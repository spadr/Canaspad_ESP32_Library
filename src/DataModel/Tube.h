#ifndef Tube_h
#define Tube_h

//#include <ArduinoJson.h>
#include "Element.h"
//#include "..\Type.h"
#include "..\ApiEndPoint.h"

class Tube
{
public:
    uuid_t token;
    String channel;
    String name;
    int size;
    bool element_empty;
    bool token_empty;
    Element element;

    float *float_value_ptr;
    int *int_value_ptr;
    long *long_value_ptr;
    unsigned int *unsigned_int_value_ptr;
    unsigned long *unsigned_long_value_ptr;

    // DONE : Create a scheme for keeping data types
    Tube(float *sensing_value);
    Tube(int *sensing_value);
    Tube(long *sensing_value);
    Tube(unsigned int *sensing_value);
    Tube(unsigned long *sensing_value);
    ~Tube();

    bool begin(String channel_, String name_);
    bool append(timestamp_tz_t timestamp);
    http_code_t send();
    void fetch(float *fresh_value);
    void fetch(int *fresh_value);
    void fetch(long *fresh_value);
    void fetch(unsigned int *fresh_value);
    void fetch(unsigned long *fresh_value);
};

#endif
