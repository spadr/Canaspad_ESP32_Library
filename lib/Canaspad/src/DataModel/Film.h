#ifndef Film_h
#define Film_h

#include "Molecule.h"
#include <ArduinoJson.h>
#include <memory>

class Film {
  private:
    std::unique_ptr<Molecule> _molecule_ptr;
    Molecule* molecule_ptr;

    String token;
    String channel;
    String name;
    bool token_empty;
    bool timestamp_empty;

    uint8_t** uint8_t_value_ptr;
    size_t* uint8_t_size;

    bool error;
    String error_message;


  public:
    // DONE : Create a scheme for keeping data types
    Film(uint8_t** sensing_value, size_t* size);
    ~Film();

    // DONE : DataModel saves and analyzes the values
    bool begin(String channel, String name, String token);
    bool add(String timestamp);
    void value(uint8_t** pick_value, size_t* size);
    String timestamp();
    String toJson();

    String checkToken() { return this->token; }
};
#endif
