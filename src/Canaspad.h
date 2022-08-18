#ifndef Canaspad_h
#define Canaspad_h

#include "DataModel\Tube.h"
#include "Type.h"

class Canaspad
{
public:
    const char *api_url;
    const char *api_key;
    const char *api_username;
    const char *api_password;
    Canaspad(const char *url, const char *key, const char *username, const char *password);
    ~Canaspad();

    bool begin();
    bool set(String const channel, String const name, Tube &sensor_config);
    bool write(struct tm &timeInfo, Tube &sensor_config);
    http_code_t send(Tube &sensor_config);
    void fetch(float *fresh_value, Tube &sensor_config);
    void fetch(int *fresh_value, Tube &sensor_config);
    void fetch(long *fresh_value, Tube &sensor_config);
    void fetch(unsigned int *fresh_value, Tube &sensor_config);
    void fetch(unsigned long *fresh_value, Tube &sensor_config);
};

#endif
