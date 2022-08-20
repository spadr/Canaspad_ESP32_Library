#include "Canaspad.h"

Canaspad::Canaspad(const char *url, const char *key, const char *username, const char *password)
{
    this->api_url = url;
    this->api_key = key;
    this->api_username = username;
    this->api_password = password;
}

Canaspad::~Canaspad()
{
    // TODO: sign out
}

bool Canaspad::begin()
{
    // TODO: sign in
    return true;
}

bool Canaspad::set(String const channel, String const name, Tube &sensor_config)
{
    bool foo = sensor_config.begin(channel, name);
    return foo;
}

bool Canaspad::write(struct tm &time_info, Tube &sensor_config)
{
    // DONE: Get timestamp from timeInfo
    struct tm now = time_info;
    char buf[20];
    sprintf(buf, " %04d-%02d-%02d %02d:%02d:%02d",
          now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
          now.tm_hour, now.tm_min, now.tm_sec);
    timestamp_tz_t timestamp = String(buf);
    return sensor_config.append(timestamp);
}

http_code_t Canaspad::send(Tube &sensor_config)
{
    http_code_t foo = sensor_config.send();
    return foo;
}

void Canaspad::fetch(float *fresh_value_p, Tube &sensor_config)
{
    sensor_config.fetch(fresh_value_p);
}

void Canaspad::fetch(int *fresh_value_p, Tube &sensor_config)
{
    sensor_config.fetch(fresh_value_p);
}

void Canaspad::fetch(long *fresh_value_p, Tube &sensor_config)
{
    sensor_config.fetch(fresh_value_p);
}

void Canaspad::fetch(unsigned int *fresh_value_p, Tube &sensor_config)
{
    sensor_config.fetch(fresh_value_p);
}

void Canaspad::fetch(unsigned long *fresh_value_p, Tube &sensor_config)
{
    sensor_config.fetch(fresh_value_p);
}
