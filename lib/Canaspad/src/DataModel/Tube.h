#ifndef Tube_h
#define Tube_h

#include "Element.h"
#include <ArduinoJson.h>
#include <memory>

typedef String json_t;

class Tube {
  private:
    std::unique_ptr<Element> _element_ptr;
    Element* element_ptr;

    uuid_t token;
    String channel;
    String name;
    bool element_empty;
    bool token_empty;
    bool timestamp_empty;

    float* float_value_ptr;
    int* int_value_ptr;
    long* long_value_ptr;
    unsigned int* unsigned_int_value_ptr;
    unsigned long* unsigned_long_value_ptr;

  public:
    // DONE : Create a scheme for keeping data types
    Tube(float* sensing_value);
    Tube(int* sensing_value);
    Tube(long* sensing_value);
    Tube(unsigned int* sensing_value);
    Tube(unsigned long* sensing_value);
    ~Tube();

    // DONE : DataModel saves and analyzes the values
    bool begin(String channel, String name, uuid_t token);
    bool add(timestamp_tz_t timestamp);
    void value(float* pick_value);
    void value(int* pick_value);
    void value(long* pick_value);
    void value(unsigned int* pick_value);
    void value(unsigned long* pick_value);
    timestamp_tz_t timestamp();
    json_t elementParse();

    json_t checkToken() { return this->token; }

    bool savedValueIs(float value) {
        float pick_value;
        this->value(&pick_value);
        return value == pick_value;
    }
    bool savedValueIs(int value) {
        int pick_value;
        this->value(&pick_value);
        return value == pick_value;
    }
    bool savedValueIs(long value) {
        long pick_value;
        this->value(&pick_value);
        return value == pick_value;
    }
    bool savedValueIs(unsigned int value) {
        unsigned int pick_value;
        this->value(&pick_value);
        return value == pick_value;
    }
    bool savedValueIs(unsigned long value) {
        unsigned long pick_value;
        this->value(&pick_value);
        return value == pick_value;
    }
    bool savedTimestampIs(timestamp_tz_t timestamp) { return timestamp == this->timestamp(); }
};

#endif
