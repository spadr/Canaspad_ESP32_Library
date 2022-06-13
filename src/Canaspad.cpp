/*
 * Canaspad.h
 */


#include "Canaspad.h"

WiFiMulti wifiMulti;

String CANASPAD_HOST  = "iot.canaspad.com";
int WIFI_TIMEOUT = 3000;

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
bool user_authentication_succeeded = false;


String ENDPOINTS_SET = "/api/set/";
String ENDPOINTS_DATA = "/api/data/";
String ENDPOINTS_IMG = "/api/image/";
String ENDPOINTS_RECEIVE = "/api/data/output/";
String ENDPOINTS_AUTH = "/api/token/";
String ENDPOINTS_REFRESH = "/api/token/refresh/";

int UUID_LENGTH = 36;
bool WIFI_CONNECTION = false;

Canaspad::Canaspad()
{

}
void
Canaspad::domain(String domain) {
  if (user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::domain()");
    Serial.println("(ERROR)Before calling the function Canaspad::begin(), this function must be called.");
    while(1){}
  }
  CANASPAD_HOST = domain;
}

void
Canaspad::wifi(const char* ssid, const char* password) {
  if (user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::wifi()");
    Serial.println("(ERROR)Before calling the function Canaspad::begin(), Canaspad::wifi() must be called.");
    while(1){}
  }
  if (wifiMulti.addAP(ssid, password)){
    WIFI_CONNECTION = true;
  }
}

bool
Canaspad::begin(const char* api_username, const char* api_password, int UTC_offset) {
  Serial.println(" ");
  Serial.print("(1/4)WiFi connecting...");

  if (CANASPAD_HOST.length() < 5){
    Serial.println("(ERROR IN)Canaspad::begin()");
    Serial.println("(ERROR)Incorrect domain");
    Serial.println("(MESSAGE)Check the host to which you are connecting.");
    Serial.println("(HOST) " + CANASPAD_HOST);
    return user_authentication_succeeded;
  }

  if (! WIFI_CONNECTION){
    Serial.println("(ERROR IN)Canaspad::begin()");
    Serial.println("(ERROR)Incorrect WiFi configuration");
    Serial.println("(MESSAGE)Check the Wi-Fi access point to which you are connecting.");
    return user_authentication_succeeded;
  }
  
  int cnt = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    cnt++;
    if (cnt>=WIFI_TIMEOUT){
      Serial.println("(ERROR IN)Canaspad::begin()");
      Serial.println("(ERROR)WiFi connection timeout");
      Serial.println("(MESSAGE)Check the Wi-Fi access point to which you are connecting.");
      return user_authentication_succeeded;
    }
  }
  Serial.println("OK");
  Serial.println("(2/4)WiFi connection succeeded");
  dif_UTC = UTC_offset;
  if(not getapitime()){
    Serial.println("(ERROR IN)Canaspad::begin()");
    Serial.println("(ERROR)API connection failed");
    Serial.println("(MESSAGE)Check the host to which you are connecting.");
    Serial.println("(HOST) " + CANASPAD_HOST);
    return user_authentication_succeeded;
  }
  Serial.println("(3/4)API connection succeeded");
  apiusername = api_username;
  apipassword = api_password;
  if(not getapiauth()){
    Serial.println("(ERROR IN)Canaspad::begin()");
    Serial.println("(ERROR)User authentication failed");
    Serial.println("(MESSAGE) " + CANASPAD_HOST + " has no record of your account.");
    Serial.println("(MESSAGE)Check the host to which you are connecting.");
    return user_authentication_succeeded;
  }
  Serial.println("(4/4)User authentication succeeded");
  Serial.println(" ");
  user_authentication_succeeded = true;
  return user_authentication_succeeded;
}

String
Canaspad::set(String device_name, String device_channel, String data_type, bool alive_monitoring, int alive_monitoring_interval){
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::set()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
  getapirefresh();
  String device_settings = "{";
  device_settings += json_format("name", device_name, false);
  device_settings += ",";
  device_settings += json_format("channel", device_channel, false);
  device_settings += ",";
  device_settings += json_format("type", data_type, false);
  device_settings += ",";
  device_settings += json_format("interval", String(alive_monitoring_interval), true);
  device_settings += ",";
  device_settings += json_format("monitoring", String(alive_monitoring), true);
  device_settings += "}";
  String json_send = "{";
  json_send += json_format("content", device_settings, true);
  json_send += "}";
  String token = postset(json_send);
  if (httpCode == 201) { //Check for the returning code
    return token;
    }
  else {
    Serial.println("(ERROR) Ch:"+device_channel+"&Name:"+device_name+" "+"No device Token.");
    return "No device Token.";
    }
}


bool
Canaspad::send(){
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::send()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
  getapirefresh();
  String json_send = "{";
  json_send += json_format("content", "[" + json_content + "]", true);
  json_send += "}";
  int _httpCode = postdata(json_send);
  if (_httpCode == 201) { //Check for the returning code
    json_content = "";
    packet_cnt = 0;
    json_flag = false;
    return true;
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

String
Canaspad::gettime(){
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::gettime()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
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
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::gettimestamp()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
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
Canaspad::add(String token, String value){
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::add()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
  if (token.length() == UUID_LENGTH){
    add_(token, value);
    return true;
  }
  else{
    Serial.println("(ERROR IN)Canaspad::add()");
    Serial.println("(ERROR)Incorrect device token");
    return false;
  }
}

bool
Canaspad::add(String token, int value){
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::add()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
  if (token.length() == UUID_LENGTH){
    add_(token, String(value));
    return true;
  }
  else{
    Serial.println("(ERROR IN)Canaspad::add()");
    Serial.println("(ERROR)Incorrect device token");
    return false;
  }
}

bool
Canaspad::add(String token, float value){
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::add()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
  if (token.length() == UUID_LENGTH){
    add_(token, String(value));
    return true;
  }
  else{
    Serial.println("(ERROR IN)Canaspad::add()");
    Serial.println("(ERROR)Incorrect device token");
    return false;
  }
}


void
Canaspad::add_(String token, String value){
  String content = "";
  content += "{";
  content += json_format("device_token", token, false);
  content += ",";
  content += json_format("data", value, false);
  content += ",";
  content += json_format("time", String(gettimestamp()), false);
  content += "}";
  if(json_flag){
    json_content += ",";
  }
  json_flag = true;
  json_content += content;
  packet_cnt++;
}

bool
Canaspad::getapitime(){
  HTTPClient http;
  StaticJsonDocument<512> time_doc;
  String url = "http://" + CANASPAD_HOST + ENDPOINTS_DATA;
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

bool
Canaspad::getapiauth(){
  HTTPClient http;
  StaticJsonDocument<2048> auth_doc;
  String auth_json = "{";
  auth_json += json_format("email", String(apiusername), false);
  auth_json += ",";
  auth_json += json_format("password", String(apipassword), false);
  auth_json += "}";
  String url = "http://" + CANASPAD_HOST + ENDPOINTS_AUTH;
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
  String refresh_url = "http://" + CANASPAD_HOST + ENDPOINTS_REFRESH;
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

int
Canaspad::postdata(String json_send){
  HTTPClient http;
  String url = "http://" + CANASPAD_HOST + ENDPOINTS_DATA;
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

String
Canaspad::postset(String json_send){
  HTTPClient http;
  String url = "http://" + CANASPAD_HOST + ENDPOINTS_SET;
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  int now = startup + millis()/1000;
  httpCode = http.POST(json_send);
  payload = http.getString();
  http.end();
  String return_data;
  if (httpCode == 201) { //Check for the returning code
    StaticJsonDocument<1024> set_doc;
    deserializeJson(set_doc, payload);
    const char* buf = set_doc["device_token"];
    return_data = String(buf);
  }
  return return_data;
}

float
Canaspad::get(String token){
  if (!user_authentication_succeeded) {
    Serial.println("(ERROR IN)Canaspad::get()");
    Serial.println("(ERROR)The Canaspad::begin() function must be called.");
    while(1){}
  }
  String json_send = "{";
  json_send += json_format("device_token", token, false);
  json_send += ",";
  json_send += json_format("lengh", "0", false);
  json_send += "}";
  getdata(json_send);
  StaticJsonDocument<2048> return_doc;
  if (httpCode == 200) {
    deserializeJson(return_doc, payload);
    float return_data  = return_doc[0]["fields"]["data"];
    return return_data;
    }
  getapiauth();
  getdata(json_send);
  if (httpCode == 200) {
    deserializeJson(return_doc, payload);
    float return_data  = return_doc[0]["fields"]["data"];
    return return_data;
    }
  return sqrt (-1);//NaN
}


void
Canaspad::getdata(String json_send){
  HTTPClient http;
  String url = "http://" + CANASPAD_HOST + ENDPOINTS_RECEIVE;
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.POST(json_send);
  payload = http.getString();
  http.end();
}