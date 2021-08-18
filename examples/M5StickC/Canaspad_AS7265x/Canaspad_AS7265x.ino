#include <M5StickC.h>
#include <Canaspad.h>
#include <Wire.h>
#include "SparkFun_AS7265X.h"

const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";

Canaspad api;
AS7265X sensor;

String sensor_410;
String sensor_435;
String sensor_460;
String sensor_485;
String sensor_510;
String sensor_535;
String sensor_560;
String sensor_585;
String sensor_610;
String sensor_645;
String sensor_680;
String sensor_705;
String sensor_730;
String sensor_760;
String sensor_810;
String sensor_860;
String sensor_900;
String sensor_940;


void setup() {
  Wire.begin(0, 26);
  
  M5.begin();
  
  Serial.begin(115200);

  Serial.print("AS7265x with M5stickC..");
  if(sensor.begin() == false)
  {
    Serial.println("Can not to be connected.");
    while(1);
  }
  Serial.print("OK");
  sensor.disableIndicator();
  
  if(not api.begin(ssid, password, 9, api_username, api_password)){
    Serial.println("Api Connection Faild");
    Serial.println(api.httpCode);
  }
  
  //Get the uuid
  sensor_410   = api.set("410nm",  "trispec", "number", 3, true);
  sensor_435   = api.set("435nm",  "trispec", "number", 3, true);
  sensor_460   = api.set("460nm",  "trispec", "number", 3, true);
  sensor_485   = api.set("485nm",  "trispec", "number", 3, true);
  sensor_510   = api.set("510nm",  "trispec", "number", 3, true);
  sensor_535   = api.set("535nm",  "trispec", "number", 3, true);
  sensor_560   = api.set("560nm",  "trispec", "number", 3, true);
  sensor_585   = api.set("585nm",  "trispec", "number", 3, true);
  sensor_610   = api.set("610nm",  "trispec", "number", 3, true);
  sensor_645   = api.set("645nm",  "trispec", "number", 3, true);
  sensor_680   = api.set("680nm",  "trispec", "number", 3, true);
  sensor_705   = api.set("705nm",  "trispec", "number", 3, true);
  sensor_730   = api.set("730nm",  "trispec", "number", 3, true);
  sensor_760   = api.set("760nm",  "trispec", "number", 3, true);
  sensor_810   = api.set("810nm",  "trispec", "number", 3, true);
  sensor_860   = api.set("860nm",  "trispec", "number", 3, true);
  sensor_900   = api.set("900nm",  "trispec", "number", 3, true);
  sensor_940   = api.set("940nm",  "trispec", "number", 3, true);
}

void loop() {
  if (api.gettimestamp() % 300 == 0){//300-second interval
    Serial.println();
    Serial.println("---------------------------------------------");
    
    //Add the measured values to JSON
    sensor.takeMeasurementsWithBulb();
    api.add(sensor.getCalibratedA(), sensor_410);
    api.add(sensor.getCalibratedB(), sensor_435);
    api.add(sensor.getCalibratedC(), sensor_460);
    api.add(sensor.getCalibratedD(), sensor_485);
    api.add(sensor.getCalibratedE(), sensor_510);
    api.add(sensor.getCalibratedF(), sensor_535);
    api.add(sensor.getCalibratedG(), sensor_560);
    api.add(sensor.getCalibratedH(), sensor_585);
    api.add(sensor.getCalibratedR(), sensor_610);
    api.add(sensor.getCalibratedI(), sensor_645);
    api.add(sensor.getCalibratedS(), sensor_680);
    api.add(sensor.getCalibratedJ(), sensor_705);
    api.add(sensor.getCalibratedT(), sensor_730);
    api.add(sensor.getCalibratedU(), sensor_760);
    api.add(sensor.getCalibratedV(), sensor_810);
    api.add(sensor.getCalibratedW(), sensor_860);
    api.add(sensor.getCalibratedK(), sensor_900);
    api.add(sensor.getCalibratedL(), sensor_940);
    
    //Send JSON to API
    if (api.send()) { 
      Serial.print("Now : ");
      Serial.println(api.gettime());//current time
      }
    else {
      int err_num = api.httpCode;
      Serial.print("Error on HTTP request! HttpCode : ");
      Serial.println(err_num);
        
      }
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}
