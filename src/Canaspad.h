#ifndef Canaspad_h
#define Canaspad_h

#include "DataModel/Tube.h"

class Canaspad {
  private:
    const char* api_url;
    const char* api_key;
    const char* api_username;
    const char* api_password;

  public:
    Canaspad(const char* url, const char* key, const char* username, const char* password,
             const long offset_sec);
    ~Canaspad();

    int offset_hour;

    int login();
    int token(String const channel, String const name, Tube& sensor);
    bool write(Tube& sensor, int year, int month, int day, int hour, int minute, int second,
               int utc_offset_hour);
    int send(Tube& sensor);
    void fetch(float* fresh_value, Tube& sensor);
    void fetch(int* fresh_value, Tube& sensor);
    void fetch(long* fresh_value, Tube& sensor);
    void fetch(unsigned int* fresh_value, Tube& sensor);
    void fetch(unsigned long* fresh_value, Tube& sensor);

    timestamp_tz_t makeTimestampTz(int year, int month, int day, int hour, int minute, int second,
                                   int utc_offset_hour);
};

#endif
