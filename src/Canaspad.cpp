/*
 * Canaspad.h
 */


#include "Canaspad.h"

String CANASPAD_HOST  = "ver3.canaspad.com";

int httpCode;
String payload;
int dif_UTC;
String json_content = "";
unsigned long startup = 0;
bool json_flag = false;
int time_offset = 0;
unsigned long packet_cnt = 0;
String refresh_token;
String access_token;
const char* apiusername;
const char* apipassword;


String ENDPOINT_USER = "/api/user/";
String ENDPOINT_TUBE = "/api/tube/";
String ENDPOINT_ELEMENTS = "/api/elements/";
String ENDPOINT_ELEMENT = "/api/element/";
String ENDPOINT_TIME = "/api/time/";
String ENDPOINT_AUTH = "/api/token/";
String ENDPOINT_REFRESH = "/api/token/refresh/";

Canaspad::Canaspad()
{

}
void
Canaspad::domain(String domain) {
  CANASPAD_HOST = domain;
}

bool
Canaspad::begin(const char ssid[], const char password[],int UTC_offset,const char* api_username, const char* api_password) {
  WiFi.begin(ssid, password);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("OK");
  dif_UTC = UTC_offset;
  if(not getapitime()){
    return false;
  }
  apiusername = api_username;
  apipassword = api_password;
  if(not getapiauth()){
    return false;
  }
  return true;
}


String
Canaspad::set(String tube_name, String tube_channel, String tube_type){
  bool isnt_send = false;
  String url = "http://" + CANASPAD_HOST + ENDPOINT_TUBE+ "?name=" + tube_name + "&channel=" + tube_channel;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  int now = startup + millis()/1000;
  httpCode = http.GET();
  payload = http.getString();
  http.end();
  String return_data;
  if (httpCode == 201) { //Check for the returning code
    StaticJsonDocument<1024> set_doc;
    deserializeJson(set_doc, payload);
    const char* buf = set_doc["tubes"][0]["token"];
    return_data = String(buf);
    return return_data;
  }
  else if(httpCode == 401){
    getapirefresh();
    String url = "http://" + CANASPAD_HOST + ENDPOINT_TUBE+ "?name=" + tube_name + "&channel=" + tube_channel;
    HTTPClient http;
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    int now = startup + millis()/1000;
    httpCode = http.GET();
    payload = http.getString();
    http.end();
    if (httpCode == 201) { //Check for the returning code
      StaticJsonDocument<1024> set_doc;
      deserializeJson(set_doc, payload);
      const char* buf = set_doc["tubes"][0]["token"];
      String return_data = String(buf);
      return return_data;
    }
    else{
      isnt_send = true;
      }
    }
  else {
    isnt_send = true;
  }

  if (isnt_send){
    String json_send = "{";
    json_send += json_format("name", tube_name, false);
    json_send += ",";
    json_send += json_format("channel", tube_channel, false);
    json_send += ",";
    json_send += json_format("type", tube_type, false);
    json_send += "}";
    HTTPClient http;
    String url = "http://" + CANASPAD_HOST + ENDPOINT_TUBE;
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    int now = startup + millis()/1000;
    httpCode = http.POST(json_send);
    payload = http.getString();
    http.end();
    if (httpCode == 201) { //Check for the returning code
      StaticJsonDocument<1024> set_doc2;
      deserializeJson(set_doc2, payload);
      const char* buf2 = set_doc2["tubes"][0]["token"];
      String return_data2 = String(buf2);
      return return_data2;
    }
    else {
    return "ERR";
    }
  }
}

void
Canaspad::add(String value, String token){
  add_(value, token);
}

void
Canaspad::add(int value, String token){
  add_(String(value), token);
}

void
Canaspad::add(float value, String token){
  add_(String(value), token);
}

void
Canaspad::add(bool value, String token){
  add_(String(value), token);
}


void
Canaspad::add_(String value, String token){
  String content = "";
  content += "{";
  content += json_format("token", token, false);
  content += ",";
  content += json_format("value", value, false);
  content += ",";
  content += json_format("time", String(gettimestamp()), false);
  content += "}";
  if(json_flag){
    json_content += ",";
  }
  json_flag = true;
  json_content += content;
  packet_cnt += 1;
}

bool
Canaspad::send(){
  String json_send = "{";
  json_send += json_format("content", "[" + json_content + "]", true);
  json_send += "}";
  httpCode = postdata(json_send);
  if (httpCode == 201) { //Check for the returning code
    json_content = "";
    packet_cnt = 0;
    json_flag = false;
    return true;
    }
  else if(httpCode == 401){
    getapirefresh();
    String json_send = "{";
    json_send += json_format("content", "[" + json_content + "]", true);
    json_send += "}";
    httpCode = postdata(json_send);
    if (httpCode == 201) { //Check for the returning code
      json_content = "";
      packet_cnt = 0;
      json_flag = false;
      return true;
      }
    else{
      if (packet_cnt >= 1000){
      json_content = "";
      packet_cnt = 0;
      json_flag = false;
    }
    return false;
      }
  }
  else {
    if (packet_cnt >= 1000){
      json_content = "";
      packet_cnt = 0;
      json_flag = false;
    }
    return false;
    }
}

int
Canaspad::postdata(String json_send){
  HTTPClient http;
  String url = "http://" + CANASPAD_HOST + ENDPOINT_ELEMENTS;
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  int now = startup + millis()/1000;
  httpCode = http.POST(json_send);
  payload = http.getString();
  http.end();
  if (httpCode == 201) { //Check for the returning code
    StaticJsonDocument<512> time_doc2;
    deserializeJson(time_doc2, payload);
    int returned = time_doc2["time"];
    time_offset = returned - now;
  }
  return httpCode;
}

bool
Canaspad::getapitime(){
  HTTPClient http;
  StaticJsonDocument<512> time_doc;
  String url = "http://" + CANASPAD_HOST + ENDPOINT_TIME;
  http.begin(url);
  httpCode = http.GET();
  if (httpCode == 200) {
    payload = http.getString();
    deserializeJson(time_doc, payload);
    int returned = time_doc["time"];
    startup = returned - millis()/1000;
    http.end();
    return true;
    }
  else {
    http.end();
    return false;
    }
}


String
Canaspad::gettime(){
  static const char *pszWDay[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  const long int t = startup + time_offset + millis()/1000 + dif_UTC * 60 * 60;
  struct tm *tm;
  tm = localtime(&t);
  char szDate[32];
  sprintf( szDate, "%04d/%02d/%02d(%s)",
          tm->tm_year+1900,
          tm->tm_mon+1,
          tm->tm_mday,
          pszWDay[tm->tm_wday] );
  char szTime[32];
  sprintf( szTime, "%02d:%02d:%02d",
          tm->tm_hour,
          tm->tm_min,
          tm->tm_sec );
  String date_time = String(szDate) + String(szTime);
  return date_time;
}

unsigned long
Canaspad::gettimestamp(){
  return startup + time_offset + millis()/1000;
}

String
Canaspad::json_format(String label, String value, bool is_list){
  String json_ = "";
  json_ += "\"";
  json_ += label;
  json_ += "\"";
  json_ += ":";
  if(is_list){
    json_ += value;
  }
  else{
  json_ += "\"";
  json_ += value;
  json_ += "\"";
  }
  return json_;
}

bool
Canaspad::getapiauth(){
  HTTPClient http;
  StaticJsonDocument<2048> auth_doc;
  String auth_json = "{";
  auth_json += json_format("email", String(apiusername), false);
  auth_json += ",";
  auth_json += json_format("password", String(apipassword), false);
  auth_json += "}";
  String url = "http://" + CANASPAD_HOST + ENDPOINT_AUTH;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.POST(auth_json);
  if (httpCode == 200) { //Check for the returning code
    payload = http.getString();
    deserializeJson(auth_doc, payload);
    const char* buf1 = auth_doc["refresh"];
    const char* buf2 = auth_doc["access"];
    refresh_token = String(buf1);
    access_token = String(buf2);
    http.end();
    return true;
    }
  else {
    http.end();
    return false;
    }
}

bool
Canaspad::getapirefresh(){
  HTTPClient http;
  StaticJsonDocument<2048> refresh_doc;
  String refresh_send = "{";
  refresh_send += json_format("refresh", refresh_token, false);
  refresh_send += "}";
  String refresh_url = "http://" + CANASPAD_HOST + ENDPOINT_REFRESH;
  http.begin(refresh_url);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.POST(refresh_send);
  if (httpCode == 200) {
    payload = http.getString();
    deserializeJson(refresh_doc, payload);
    const char* buf = refresh_doc["access"];
    access_token = String(buf);
    http.end();
    return true;
    }
  else {
    http.end(); 
    return getapiauth();
    }
}

float
Canaspad::get_float(String token){
  String url = "http://" + CANASPAD_HOST + ENDPOINT_ELEMENT + "?token=" + token + "&length=" + "1";
  HTTPClient http;
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.GET();
  payload = http.getString();
  http.end();
  StaticJsonDocument<2048> return_doc;
  float return_data;
  if (httpCode == 200) {
    deserializeJson(return_doc, payload);
    return_data  = return_doc["number"][0]["value"];
    }
  else {
    getapiauth();
    HTTPClient http;
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    httpCode = http.GET();
    payload = http.getString();
    http.end();
    deserializeJson(return_doc, payload);
    return_data = return_doc["number"][0]["value"];
    }
  return return_data;
  }

bool
Canaspad::get_boolean(String token){
  String url = "http://" + CANASPAD_HOST + ENDPOINT_ELEMENT+ "?token=" + token + "&length=" + "1";
  HTTPClient http;
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.GET();
  payload = http.getString();
  http.end();
  StaticJsonDocument<2048> return_doc;
  bool return_data;
  if (httpCode == 200) {
    deserializeJson(return_doc, payload);
    const char* buf = return_doc["boolean"][0]["value"];
    return_data = bool(buf);
    }
  else {
    getapiauth();
    HTTPClient http;
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    httpCode = http.GET();
    payload = http.getString();
    http.end();
    deserializeJson(return_doc, payload);
    return_data = return_doc["boolean"][0]["value"];
    }
  return return_data;
  }
  
String
Canaspad::get_string(String token){
  String url = "http://" + CANASPAD_HOST + ENDPOINT_ELEMENT+ "?token=" + token + "&length=" + "1";
  HTTPClient http;
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.GET();
  payload = http.getString();
  http.end();
  StaticJsonDocument<2048> return_doc;
  String return_data;
  if (httpCode == 200) {
    deserializeJson(return_doc, payload);
    const char* buf = return_doc["char"][0]["value"];
    return_data = String(buf);
    }
  else {
    getapiauth();
    HTTPClient http;
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    httpCode = http.GET();
    payload = http.getString();
    http.end();
    deserializeJson(return_doc, payload);
    const char* buf = return_doc["char"][0]["value"];
    return_data = String(buf);
    }
  return return_data;
  }