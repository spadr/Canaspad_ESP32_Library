#include <M5StickCPlus.h>
#include <Canaspad.h>
#include "VL53L0X.h"

const char* ssid         = "WiFi_ssid";
const char* password     = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";

unsigned long interval = 60*5;//Seconds(s)

Canaspad api;
VL53L0X tof;

String sensor_distance;

float distance = 0.0;


void setup() {
  M5.begin();
  
  Serial.begin(115200);

  //api.domain("own.server.com");//Sending to your own server
  
  api.wifi(ssid, password);//Add WiFi configuration
  //api.wifi("ssid2", "password2");//Add multiple WiFi settings.
  
  if(not api.begin(api_username, api_password)){
    Serial.println("Connection Faild");
  }
  
  //Getting a uuid
  sensor_distance   = api.set("TOF",  "distance");

  //Initializing the sensor
  tof.setAddress(0x29);
  tof.setTimeout(500);
  if (!tof.init()) {
    Serial.println("Failed to connect sensor!");
    while(1){}
  }
  tof.startContinuous();
}

void loop() {
  if (api.gettimestamp() % interval == 0){//interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Get the measured value
    distance = tof.readRangeContinuousMillimeters();
    
    Serial.printf("Distance: %2.2fmm\r\n", distance);

    //Add the measured values to JSON
    api.add(distance, sensor_distance);

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

    //Getting values from API
    float res_distance =  api.get(sensor_distance);
    
    Serial.printf("Distance: %2.2fmm(Received from the API)\r\n", res_distance);
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}
