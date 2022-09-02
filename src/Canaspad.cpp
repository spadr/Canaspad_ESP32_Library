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

bool Canaspad::write(struct tm &time_info, Tube &sensor)
{
    // DONE: Get timestamp from timeInfo
    struct tm now = time_info;
    char buf[23];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d+%02d",
            now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
            now.tm_hour, now.tm_min, now.tm_sec, Canaspad::offset_hour);
    timestamp_tz_t timestamp = String(buf);
    return sensor.add(timestamp);
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
