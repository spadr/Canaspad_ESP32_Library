#ifndef Molecule_h
#define Molecule_h

#include <Arduino.h>
#include <memory>


class Molecule {
  private:
    String timestamp;
    uint8_t* value = nullptr;
    size_t size;

  public:
    Molecule();
    ~Molecule();

    bool append(uint8_t** add_value, size_t* size, String timestamp_add);
    bool remove(uint8_t** _value, size_t* _size);

    String savedTimestamp();

    bool savedTimestampIs(String timestamp);
};

#endif
