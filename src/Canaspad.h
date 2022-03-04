/*
 * Canaspad.h
 */

#ifndef Canaspad_h
#define Canaspad_h

#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <ArduinoJson.h>

class Canaspad {
public:
  Canaspad();
  void domain(String domain);
  bool begin(const char ssid[], const char password[],int UTC_offset, const char* api_username, const char* api_password);
  String set(String tube_name, String tube_channel, String tube_type);
  void add(String value, String token);
  void add(int value, String token);
  void add(float value, String token);
  void add(bool value, String token);
  bool send();
  float get_float(String token);
  bool get_boolean(String token);
  String get_string(String token);
  int32_t image_post( uint8_t * ui8BufJpg, uint32_t iNumDat );
  String gettime();
  unsigned long gettimestamp();
  String payload;
  String json_content;
  unsigned long startup;
  int httpCode;
  
private:
  int dif_UTC;
  bool json_flag;
  int time_offset;
  String json_format(String label, String value, bool is_list);
  bool getapitime();
  bool getapiauth();
  bool getapirefresh();
  int postdata(String json_send);
  const char* apiusername;
  const char* apipassword;
  void add_(String value, String token);
};

#endif // Canaspad_h