#include <M5StickC.h>
#include <Canaspad.h>
#include "SparkFun_I2C_Mux_Arduino_Library.h"

const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";

unsigned long interval = 60*30;//Seconds(s)

Canaspad api;
QWIICMUX pahub;

String sensor_val0, sensor_val1, sensor_val2, sensor_val3, sensor_val4, sensor_val5;

float val0, val1, val2, val3, val4, val5;


void setup() {
  M5.begin();
  
  Serial.begin(115200);

  //Connecting to the API
  if(not api.begin(ssid, password, 9, api_username, api_password)){
    Serial.println("Failed to connect API!");
    Serial.println(api.httpCode);
    while(1){}
  }
  Serial.println("API connection succeeded.");
  
  //Getting a uuid
  sensor_val0   = api.set("Port0",  "Pa.hub", "number");
  sensor_val1   = api.set("Port1",  "Pa.hub", "number");
  sensor_val2   = api.set("Port2",  "Pa.hub", "number");
  sensor_val3   = api.set("Port3",  "Pa.hub", "number");
  sensor_val4   = api.set("Port4",  "Pa.hub", "number");
  sensor_val5   = api.set("Port5",  "Pa.hub", "number");

  //Initializing the Pa.hub
  Wire.begin(32,33);
  if (pahub.begin(0x70, Wire) == false)
  {
    Serial.println("Pa.hub not detected!");
    while(1){}
  }
  Serial.println("Pa.hub detected!");

  //Initializing the sensor of port 0
  pahub.setPort(0);

  //Initializing the sensor of port 1
  pahub.setPort(1);

  //Initializing the sensor of port 2
  pahub.setPort(2);

  //Initializing the sensor of port 3
  pahub.setPort(3);

  //Initializing the sensor of port 4
  pahub.setPort(4);
  
  //Initializing the sensor of port 5
  pahub.setPort(5);
  
}

void loop() {
  if (api.gettimestamp() % interval == 0){//interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Getting the measurement value of port 0
    pahub.setPort(0);
    val0 = 0.0;
    //Getting the measurement value of port 1
    pahub.setPort(1);
    val1 = 1.0;
    //Getting the measurement value of port 2
    pahub.setPort(2);
    val2 = 2.0;
    //Getting the measurement value of port 3
    pahub.setPort(3);
    val3 = 3.0;
    //Getting the measurement value of port 4
    pahub.setPort(4);
    val4 = 4.0;
    //Getting the measurement value of port 5
    pahub.setPort(5);
    val5 = 5.0;
    
    //Add the measured values to JSON
    api.add(val0, sensor_val0);
    api.add(val1, sensor_val1);
    api.add(val2, sensor_val2);
    api.add(val3, sensor_val3);
    api.add(val4, sensor_val4);
    api.add(val5, sensor_val5);

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
    delay(10*1000);
  }
}
