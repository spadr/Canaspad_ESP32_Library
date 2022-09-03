#include "Canaspad.h"
#include "ApiEndPoint.h"

Canaspad::Canaspad(const char *url, const char *key, const char *username, const char *password, const long offset_sec)
{
    this->api_url = url;
    this->api_key = key;
    this->api_username = username;
    this->api_password = password;
    this->offset_hour = offset_sec / 3600;
}

Canaspad::~Canaspad()
{
    // TODO: sign out
}

int Canaspad::login()
{
    // TODO: sign in
    return HttpStatus::toInt(HttpStatus::Code::NotFound);
}

int Canaspad::token(String const channel, String const name, Tube &sensor)
{
    uuid_t token = "token"; // TODO: Get TUBE record if channel and name exist
    sensor.begin(channel, name, token);
    return HttpStatus::toInt(HttpStatus::Code::NotFound);
}

bool Canaspad::write(Tube &sensor, int year, int month, int day, int hour, int minute, int second, int utc_offset_hour)
{
    timestamp_tz_t timestamp_with_tz = _make_timestamp_tz(year, month, day, hour, minute, second, utc_offset_hour);
    return sensor.add(timestamp_with_tz);
}

int Canaspad::send(Tube &sensor)
{
    float _value;
    sensor.latest_value(&_value);
    timestamp_tz_t _timestamp = sensor.latest_timestamp();
    return HttpStatus::toInt(HttpStatus::Code::NotFound);
}

void Canaspad::fetch(float *fresh_value_p, Tube &sensor)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value_p = 3.9;
}

void Canaspad::fetch(int *fresh_value_p, Tube &sensor)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value_p = -39;
}

void Canaspad::fetch(long *fresh_value_p, Tube &sensor)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value_p = -339;
}

void Canaspad::fetch(unsigned int *fresh_value_p, Tube &sensor)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value_p = 39;
}

void Canaspad::fetch(unsigned long *fresh_value_p, Tube &sensor)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value_p = 339;
}

timestamp_tz_t Canaspad::_make_timestamp_tz(int year, int month, int day, int hour, int minute, int second, int utc_offset_hour)
{
    // DONE: make timestamp
    char buf[23];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d+%02d", year, month, day, hour, minute, second, utc_offset_hour);
    timestamp_tz_t timestamp = String(buf);
    return timestamp;
}