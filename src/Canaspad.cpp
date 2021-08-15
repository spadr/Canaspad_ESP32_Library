/*
 * Canaspad.h
 */


#include "Canaspad.h"

const char* CANASPAD_HOST  = "192.168.10.117";

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

String ENDPOINTS_SET = "/api/set/";
String ENDPOINTS_DATA = "/api/data/";
String ENDPOINTS_IMG = "/api/image/";
String ENDPOINTS_RECEIVE = "/api/data/output/";
String ENDPOINTS_AUTH = "/api/token/";
String ENDPOINTS_REFRESH = "/api/token/refresh/";

Canaspad::Canaspad()
{

}


bool
Canaspad::begin(const char ssid[], const char password[],int UTC_offset,const char* api_username, const char* api_password) {
  Serial.println("------------------------------");
  WiFi.begin(ssid, password);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
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
Canaspad::set(String device_name, String device_channel, String device_type, int alive_interval, bool monitoring){
  getapirefresh();
  String device_settings = "{";
  device_settings += json_format("name", device_name, false);
  device_settings += ",";
  device_settings += json_format("channel", device_channel, false);
  device_settings += ",";
  device_settings += json_format("type", device_type, false);
  device_settings += ",";
  device_settings += json_format("interval", String(alive_interval), true);
  device_settings += ",";
  device_settings += json_format("monitoring", String(monitoring), true);
  device_settings += "}";
  String json_send = "{";
  json_send += json_format("content", device_settings, true);
  json_send += "}";
  String token = postset(json_send);
  if (httpCode == 201) { //Check for the returning code
    Serial.println("SETTING OK");
    Serial.println("------------------------------");
    return token;
    }
  else {
    Serial.println("SETTING NG");
    Serial.println("------------------------------");
    return "Nan";
    }
}


bool
Canaspad::send(){
  getapirefresh();
  String json_send = "{";
  json_send += json_format("content", "[" + json_content + "]", true);
  json_send += "}";
  httpCode = postdata(json_send);
  if (httpCode == 201) { //Check for the returning code
    json_content = "";
    packet_cnt = 0;
    json_flag = false;
    Serial.println("Send OK");
    Serial.println("Clear Data");
    Serial.println("------------------------------");
    return true;
    }
  else {
    if (packet_cnt >= 1000){
      json_content = "";
      packet_cnt = 0;
      json_flag = false;
      Serial.println("Clear Data");
    }
    Serial.println("Send NG");
    Serial.println("------------------------------");
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
Canaspad::add(String value, String token){
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
  packet_cnt += 1;
  Serial.print("Add Data : ");
  Serial.println(content);
  Serial.print("Add Counter : ");
  Serial.println(packet_cnt);
  Serial.println("------------------------------");
}

bool
Canaspad::getapitime(){
  HTTPClient http;
  StaticJsonDocument<512> time_doc;
  String url = "http://" + String(CANASPAD_HOST) + ENDPOINTS_DATA;
  Serial.print("Request URL : ");
  Serial.println(url);
  http.begin(url);
  httpCode = http.GET();
  Serial.print("Status Code : ");
  Serial.println(httpCode);
  if (httpCode == 200) {
    payload = http.getString();
    deserializeJson(time_doc, payload);
    int returned = time_doc["time"];
    startup = returned - millis()/1000;
    http.end();
    Serial.print("Now : ");
    Serial.println(startup);
    Serial.println("API Connection OK");
    Serial.println("------------------------------");
    return true;
    }
  else {
    http.end();
    Serial.println("API Connection NG");
    Serial.println("------------------------------");
    return false;
    }
}

bool
Canaspad::getapiauth(){
  HTTPClient http;
  StaticJsonDocument<2048> auth_doc;
  String auth_json = "{";
  auth_json += json_format("username", String(apiusername), false);
  auth_json += ",";
  auth_json += json_format("password", String(apipassword), false);
  auth_json += "}";
  String url = "http://" + String(CANASPAD_HOST) + ENDPOINTS_AUTH;
  Serial.print("Request URL : ");
  Serial.println(url);
  Serial.print("POST Payload: ");
  Serial.println(auth_json);
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
    Serial.print("Refresh Token: ");
    Serial.println(refresh_token);
    Serial.print("Access Token: ");
    Serial.println(access_token);
    Serial.println("Auth Connection OK");
    Serial.println("------------------------------");
    return true;
    }
  else {
    http.end();
    Serial.println("Auth Connection NG");
    Serial.println("------------------------------");
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
  String refresh_url = "http://" + String(CANASPAD_HOST) + ENDPOINTS_REFRESH;
  Serial.print("Request URL : ");
  Serial.println(refresh_url);
  http.begin(refresh_url);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.POST(refresh_send);
  if (httpCode == 200) {
    payload = http.getString();
    deserializeJson(refresh_doc, payload);
    const char* buf = refresh_doc["access"];
    access_token = String(buf);
    http.end();
    Serial.print("Access Token: ");
    Serial.println(access_token);
    Serial.println("Refresh Connection OK");
    Serial.println("------------------------------");
    return true;
    }
  else {
    http.end(); 
    Serial.println("Refresh Connection NG THEN");
    Serial.println("------------------------------");
    return getapiauth();
    }
}

int
Canaspad::postdata(String json_send){
  HTTPClient http;
  String url = "http://" + String(CANASPAD_HOST) + ENDPOINTS_DATA;
  Serial.print("Send URL : ");
  Serial.println(url);
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  int now = startup + millis()/1000;
  httpCode = http.POST(json_send);
  payload = http.getString();
  http.end();
  Serial.print("Send Data: ");
  Serial.println(json_send);
  Serial.println(httpCode);
  Serial.println(now);
  if (httpCode == 201) { //Check for the returning code
    StaticJsonDocument<512> time_doc2;
    deserializeJson(time_doc2, payload);
    int returned = time_doc2["time"];
    time_offset = returned - now;
    Serial.print("Time Offset: ");
    Serial.println(time_offset);
  }
  return httpCode;
}

String
Canaspad::postset(String json_send){
  HTTPClient http;
  String url = "http://" + String(CANASPAD_HOST) + ENDPOINTS_SET;
  Serial.print("Send URL : ");
  Serial.println(url);
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  int now = startup + millis()/1000;
  httpCode = http.POST(json_send);
  payload = http.getString();
  http.end();
  Serial.print("Send Data: ");
  Serial.println(json_send);
  Serial.println(httpCode);
  Serial.println(now);
  String return_data;
  if (httpCode == 201) { //Check for the returning code
    StaticJsonDocument<1024> set_doc;
    deserializeJson(set_doc, payload);
    const char* buf = set_doc["device_token"];
    return_data = String(buf);
    Serial.print("device_token: ");
    Serial.println(return_data);
  }
  return return_data;
}

float
Canaspad::get(String token){
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
    Serial.print("Return Data: ");
    Serial.println(return_data);
    Serial.println("GET Data Connection OK");
    Serial.println("------------------------------");
    return return_data;
    }
  else {
    Serial.println("GET Data Connection NG THEN");
    Serial.println("------------------------------");
    getapiauth();
    getdata(json_send);
    deserializeJson(return_doc, payload);
    float return_data  = return_doc[0]["fields"]["data"];
    Serial.print("Return Data: ");
    Serial.println(return_data);
    Serial.println("GET Data Connection OK");
    Serial.println("------------------------------");
    return return_data;
    }
}


void
Canaspad::getdata(String json_send){
  HTTPClient http;
  String url = "http://" + String(CANASPAD_HOST) + ENDPOINTS_RECEIVE;
  Serial.print("Send URL : ");
  Serial.println(url);
  http.begin(url);
  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.POST(json_send);
  payload = http.getString();
  http.end();
  Serial.print("Send Data: ");
  Serial.println(json_send);
  Serial.println(httpCode);
  Serial.println(payload);
  Serial.println("------------------------------");
}
