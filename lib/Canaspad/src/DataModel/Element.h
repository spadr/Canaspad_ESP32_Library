#ifndef Element_h
#define Element_h

#include "Nucleus.h"
#include <Arduino.h>
#include <memory>


class Element {
  private:
    std::unique_ptr<Nucleus> _nucleus_ptr;
    Nucleus* nucleus_ptr;

    String timestamp;

  public:
    Element();
    ~Element();

    bool append(float* add_value, String timestamp_add);
    bool append(int* add_value, String timestamp_add);
    bool append(long* add_value, String timestamp_add);
    bool append(unsigned int* add_value, String timestamp_add);
    bool append(unsigned long* add_value, String timestamp_add);

    bool savedValue(float* _value);
    bool savedValue(int* _value);
    bool savedValue(long* _value);
    bool savedValue(unsigned int* _value);
    bool savedValue(unsigned long* _value);
    String savedTimestamp();

    bool savedValueIs(float value);
    bool savedValueIs(int value);
    bool savedValueIs(long value);
    bool savedValueIs(unsigned int value);
    bool savedValueIs(unsigned long value);
    bool savedTimestampIs(String timestamp);
};

#endif
