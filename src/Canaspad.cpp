#include "Canaspad.h"
#include "ApiEndPoint.h"

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

http_code_t Canaspad::login()
{
    // TODO: sign in
    http_code_t r = "Disconnected";
    return r;
}

http_code_t Canaspad::token(String const channel, String const name, Tube &sensor)
{
    bool foo = sensor.begin(channel, name);
    http_code_t r = "Disconnected";
    return r;
}

bool Canaspad::write(struct tm &time_info, Tube &sensor)
{
    // DONE: Get timestamp from timeInfo
    struct tm now = time_info;
    char buf[20];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
          now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
          now.tm_hour, now.tm_min, now.tm_sec);
    timestamp_tz_t timestamp = String(buf);
    return sensor.add(timestamp);
    
}

http_code_t Canaspad::send(Tube &sensor)
{
    http_code_t foo = sensor.send();
    http_code_t r = "Disconnected";
    return r;
}

void Canaspad::fetch(float *fresh_value_p, Tube &sensor)
{
    sensor.fetch(fresh_value_p);
}

void Canaspad::fetch(int *fresh_value_p, Tube &sensor)
{
    sensor.fetch(fresh_value_p);
}

void Canaspad::fetch(long *fresh_value_p, Tube &sensor)
{
    sensor.fetch(fresh_value_p);
}

void Canaspad::fetch(unsigned int *fresh_value_p, Tube &sensor)
{
    sensor.fetch(fresh_value_p);
}

void Canaspad::fetch(unsigned long *fresh_value_p, Tube &sensor)
{
    sensor.fetch(fresh_value_p);
}

