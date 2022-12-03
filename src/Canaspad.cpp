#include "Canaspad.h"


Canaspad::Canaspad(const char* host, const char* key, const char* username, const char* password,
                   const long offset_sec) {
    this->api_host = host;
    this->api_key = key;
    this->api_username = username;
    this->api_password = password;
    this->offset_hour = offset_sec / 3600;
}

Canaspad::~Canaspad() {
    // TODO: sign out
}

int Canaspad::login() {
    // TODO: sign in
    supabase.begin(this->api_host, this->api_key, this->api_username, this->api_password,
                   canaspad_api_end_point.auth);
    return HttpStatus::toInt(HttpStatus::Code::NotFound);
}

int Canaspad::token(String const channel, String const name, Tube& sensor) {
    uuid_t token = "token"; // TODO: Get TUBE record if channel and name exist
    sensor.begin(channel, name, token);
    return HttpStatus::toInt(HttpStatus::Code::NotFound);
}

bool Canaspad::write(Tube& sensor, int year, int month, int day, int hour, int minute, int second,
                     int utc_offset_hour) {
    timestamp_tz_t timestamp_with_tz =
        makeTimestampTz(year, month, day, hour, minute, second, utc_offset_hour);
    return sensor.add(timestamp_with_tz);
}

int Canaspad::send(Tube& sensor) {
    // TODO: Post Tube record
    timestamp_tz_t _timestamp = sensor.timestamp();
    json_t json = sensor.elementParse();
    // HttpResponse response = supabase.from("element").insert(json).execute();
    ////HttpResponse response = supabase.execute();
    return 1; ////response.status_code;
}

void Canaspad::fetch(float* fresh_value_p, Tube& sensor) {
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    // HttpResponse response = supabase.from("fresh_elements").select("*").execute();
    // Json json = deserialize(response.message)
    //*fresh_value_p = json[value];
    *fresh_value_p = 3.9;
}

void Canaspad::fetch(int* fresh_value_p, Tube& sensor) {
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    // HttpResponse response = supabase.from("fresh_elements").select("*").execute();
    // Json json = deserialize(response.message)
    //*fresh_value_p = json[value];
    *fresh_value_p = -39;
}

void Canaspad::fetch(long* fresh_value_p, Tube& sensor) {
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    // HttpResponse response = supabase.from("fresh_elements").select("*").execute();
    // Json json = deserialize(response.message)
    //*fresh_value_p = json[value];
    *fresh_value_p = -339;
}

void Canaspad::fetch(unsigned int* fresh_value_p, Tube& sensor) {
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    // HttpResponse response = supabase.from("fresh_elements").select("*").execute();
    // Json json = deserialize(response.message)
    //*fresh_value_p = json[value];
    *fresh_value_p = 39;
}

void Canaspad::fetch(unsigned long* fresh_value_p, Tube& sensor) {
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    // HttpResponse response = supabase.from("fresh_elements").select("*").execute();
    // Json json = deserialize(response.message)
    //*fresh_value_p = json[value];
    *fresh_value_p = 339;
}

timestamp_tz_t Canaspad::makeTimestampTz(int year, int month, int day, int hour, int minute,
                                         int second, int utc_offset_hour) {
    // DONE: make timestamp
    char buf[23];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d+%02d", year, month, day, hour, minute, second,
            utc_offset_hour);
    timestamp_tz_t timestamp = String(buf);
    return timestamp;
}