#include "Film.h"

Film::Film(uint8_t** sensing_value_buffer, size_t* buffer_size) {
    this->uint8_t_value_ptr = sensing_value_buffer;
    this->uint8_t_size = buffer_size;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->token_empty = true;
    this->timestamp_empty = true;

    this->_molecule_ptr = std::unique_ptr<Molecule>(new Molecule());
    this->molecule_ptr = _molecule_ptr.get();
}

Film::~Film() {
    if (this->_molecule_ptr) {
        _molecule_ptr.reset(nullptr);
    }
}

bool Film::begin(String channel, String name, String token) {
    this->channel = channel;
    this->name = name;
    this->token = token;
    this->token_empty = false;
    return true;
}

bool Film::add(String timestamp) {
    if (this->uint8_t_value_ptr != nullptr && this->uint8_t_size != nullptr) {
        this->timestamp_empty = false;
        return molecule_ptr->append(uint8_t_value_ptr, uint8_t_size, timestamp);
    }
    // Error
    return false;
}

String Film::timestamp() {
    if (this->timestamp_empty) {
        // Error
    }
    return molecule_ptr->savedTimestamp();
}

void Film::value(uint8_t** pick_value, size_t* size) { molecule_ptr->remove(pick_value, size); }

String Film::toJson() {
    if (!this->token_empty && !this->timestamp_empty) {
        DynamicJsonDocument doc(256);
        doc["film_token"] = this->token;
        doc["created_at"] = molecule_ptr->savedTimestamp();
        doc["is_delete"] = true;
        String output;
        serializeJson(doc, output);
        return output;
    } else {
        // Error
        return "";
    }
    return "";
}