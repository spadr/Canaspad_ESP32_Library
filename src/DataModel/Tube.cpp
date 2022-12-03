#include "Tube.h"

Tube::Tube(float* sensing_value) {
    this->float_value_ptr = sensing_value;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->element_empty = true;
    this->token_empty = true;
    this->timestamp_empty = true;
}

Tube::Tube(int* sensing_value) {
    this->float_value_ptr = nullptr;
    this->int_value_ptr = sensing_value;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->element_empty = true;
    this->token_empty = true;
    this->timestamp_empty = true;
}

Tube::Tube(long* sensing_value) {
    this->float_value_ptr = nullptr;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = sensing_value;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->element_empty = true;
    this->token_empty = true;
    this->timestamp_empty = true;
}

Tube::Tube(unsigned int* sensing_value) {
    this->float_value_ptr = nullptr;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = sensing_value;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->element_empty = true;
    this->token_empty = true;
    this->timestamp_empty = true;
}

Tube::Tube(unsigned long* sensing_value) {
    this->float_value_ptr = nullptr;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = sensing_value;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->element_empty = true;
    this->token_empty = true;
    this->timestamp_empty = true;
}

Tube::~Tube() {}

bool Tube::begin(String channel, String name, uuid_t token) {
    this->channel = channel;
    this->name = name;
    this->token = token;
    this->token_empty = false;
    return true;
}

bool Tube::add(timestamp_tz_t timestamp) {
    // TODO : Test
    if (float_value_ptr != nullptr) {
        element_empty = false;
        timestamp_empty = false;
        return Element::append(float_value_ptr, timestamp);
    } else if (int_value_ptr != nullptr) {
        element_empty = false;
        timestamp_empty = false;
        return Element::append(int_value_ptr, timestamp);
    } else if (long_value_ptr != nullptr) {
        element_empty = false;
        timestamp_empty = false;
        return Element::append(long_value_ptr, timestamp);
    } else if (unsigned_int_value_ptr != nullptr) {
        element_empty = false;
        timestamp_empty = false;
        return Element::append(unsigned_int_value_ptr, timestamp);
    } else if (unsigned_long_value_ptr != nullptr) {
        element_empty = false;
        timestamp_empty = false;
        return Element::append(unsigned_long_value_ptr, timestamp);
    } else {
        // Error
        return false;
    }
}

void Tube::value(float* pick_value) {
    if (element_empty) {
        // Error
    }
    Element::savedValue(pick_value);
}

void Tube::value(int* pick_value) {
    if (element_empty) {
        // Error
    }
    Element::savedValue(pick_value);
}

void Tube::value(long* pick_value) {
    if (element_empty) {
        // Error
    }
    Element::savedValue(pick_value);
}

void Tube::value(unsigned int* pick_value) {
    if (element_empty) {
        // Error
    }
    Element::savedValue(pick_value);
}

void Tube::value(unsigned long* pick_value) {
    if (element_empty) {
        // Error
    }
    Element::savedValue(pick_value);
}

timestamp_tz_t Tube::timestamp() {
    if (timestamp_empty) {
        // Error
    }
    return Element::savedTimestamp();
}


json_t Tube::elementParse() {
    if (!token_empty && !element_empty && !timestamp_empty) {
        StaticJsonDocument<200> doc;
        doc["token"] = this->token;
        doc["timestamp"] = Element::savedTimestamp();
        if (float_value_ptr != nullptr) {
            float value;
            Element::savedValue(&value);
            doc["value"] = value; // Only 9 decimal places
        } else if (int_value_ptr != nullptr) {
            int value;
            Element::savedValue(&value);
            doc["value"] = value;
        } else if (long_value_ptr != nullptr) {
            long value;
            Element::savedValue(&value);
            doc["value"] = value;
        } else if (unsigned_int_value_ptr != nullptr) {
            unsigned int value;
            Element::savedValue(&value);
            doc["value"] = value;
        } else if (unsigned_long_value_ptr != nullptr) {
            unsigned long value;
            Element::savedValue(&value);
            doc["value"] = value;
        } else {
            // Error
        }
        String output;
        serializeJson(doc, output);
        json_t json = output;
        return json;
    } else {
        // Error
        return "";
    }
    return "";
}
