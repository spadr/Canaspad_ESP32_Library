#ifndef Canaspad_h
#define Canaspad_h

#include "ApiEndPoint.h"
#include "DataModel/Film.h"
#include "DataModel/Tube.h"
#include <ArduinoJson.h>
#include <Supabase.h>


class Canaspad {
  protected:
    CanaspadEndPoint canaspad_api_end_point;
    Supabase supabase;

  private:
    const char* api_host;
    const char* api_username;
    const char* api_password;

    String error_message;

    bool createConnection(Tube& sensor, String const channel, String const name);
    bool createConnection(Film& sensor, String const channel, String const name);

  public:
    Canaspad(const char* host, const char* username, const char* password, const long offset_sec,
             const char* root_ca = nullptr, const char* client_cert = nullptr,
             const char* client_key = nullptr);
    ~Canaspad();

    int offset_hour;

    bool login();

    bool connect(Tube& sensor, String const channel, String const name);
    bool write(Tube& sensor, int year, int month, int day, int hour, int minute, int second,
               int utc_offset_hour);
    bool send(Tube& sensor);
    bool fetch(Tube& sensor, float* fresh_value_p, String* fresh_timestamp_p = nullptr);
    bool fetch(Tube& sensor, int* fresh_value_p, String* fresh_timestamp_p = nullptr);
    bool fetch(Tube& sensor, long* fresh_value_p, String* fresh_timestamp_p = nullptr);
    bool fetch(Tube& sensor, unsigned int* fresh_value_p, String* fresh_timestamp_p = nullptr);
    bool fetch(Tube& sensor, unsigned long* fresh_value_p, String* fresh_timestamp_p = nullptr);

    bool connect(Film& sensor, String const channel, String const name);
    bool write(Film& sensor, int year, int month, int day, int hour, int minute, int second,
               int utc_offset_hour);
    bool send(Film& sensor);
    bool fetch(Film& sensor, uint8_t* fresh_value_p, String* fresh_timestamp_p = nullptr);


    String makeTimestampTz(int year, int month, int day, int hour, int minute, int second,
                           int utc_offset_hour);

    String checkErrorMessage() { return error_message; }
};

#endif
