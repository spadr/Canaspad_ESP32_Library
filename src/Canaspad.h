/*
 * Canaspad.h
 */

#ifndef Canaspad_h
#define Canaspad_h

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <time.h>
#include <ArduinoJson.h>

class Canaspad {
public:
  Canaspad();
  void domain(String domain);
  void wifi(const char* ssid, const char* password);
  bool begin(const char* api_username, const char* api_password, int UTC_offset);
  String set(String device_name, String device_channel, String device_type, int interval, bool monitoring);
  bool send();
  void add(String value, String token);
  void add(int value, String token);
  void add(float value, String token);
  float get(String token);
  void getdata(String json_send);
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
  String postset(String json_send);
  const char* apiusername;
  const char* apipassword;
  void add_(String value, String token);
};

#endif // Canaspad_h