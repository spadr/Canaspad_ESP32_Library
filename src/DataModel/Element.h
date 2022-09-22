#ifndef Element_h
#define Element_h

#include "../Type.h"
#include "Nucleus.h"

class Element {
  private:
    Nucleus nucleus;
    timestamp_tz_t timestamp;

  public:
    Element();
    ~Element();

    bool append(float* add_value, timestamp_tz_t timestamp_add);
    bool append(int* add_value, timestamp_tz_t timestamp_add);
    bool append(long* add_value, timestamp_tz_t timestamp_add);
    bool append(unsigned int* add_value, timestamp_tz_t timestamp_add);
    bool append(unsigned long* add_value, timestamp_tz_t timestamp_add);

    bool savedValue(float* _value);
    bool savedValue(int* _value);
    bool savedValue(long* _value);
    bool savedValue(unsigned int* _value);
    bool savedValue(unsigned long* _value);
    timestamp_tz_t savedTimestamp();

    bool savedValueIs(float value);
    bool savedValueIs(int value);
    bool savedValueIs(long value);
    bool savedValueIs(unsigned int value);
    bool savedValueIs(unsigned long value);
    bool savedTimestampIs(timestamp_tz_t timestamp);
};

#endif
