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

bool Canaspad::write(struct tm &timeInfo, Tube &sensor_config)
{
    // TODO: Get timestamp from timeInfo
    timestamp_tz_t timestamp = "2004-10-19 10:23:54+09";
    bool foo = sensor_config.append(timestamp);
    return foo;
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
