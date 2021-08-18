#include <M5StickC.h>
#include <Canaspad.h>
#include "Seeed_SHT35.h"

#define SDAPIN 32
#define SCLPIN 33

const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";

Canaspad api;
SHT35 sensor(SCLPIN);

String sensor_tmp;
String sensor_hum;
String sensor_housa;

float tmp = 0.0;
float hum = 0.0;
float housa = 0.0;

void setup() {
  M5.begin();
  
  Serial.begin(115200);
  delay(10);
    
  Serial.print("SHT35 Serial START.");
  if (sensor.init()) {
    Serial.println("Sensor init FAILED");
  }
  Serial.println("OK");
  
  if(not api.begin(ssid, password, 9, api_username, api_password)){
    Serial.println("Api Connection Faild");
    Serial.println(api.httpCode);
  }
  
  //Get the uuid
  sensor_tmp   = api.set("tmp",  "SHT35", "number", 3, true);
  sensor_hum   = api.set("hum",  "SHT35", "number", 3, true);
  sensor_housa   = api.set("housa",  "SHT35", "number", 3, true);
}

void loop() {
  if (api.gettimestamp() % 60 == 0){//60-second interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Get the measured value
    if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &tmp, &hum)) {
      Serial.println("SHT35 Sensing FAILED!!");

      //Add the measured values to JSON
      api.add("nan", sensor_tmp);
      api.add("nan", sensor_hum);
      api.add("nan", sensor_housa);
    }
    else{
      housa = (100-hum)*(217*(6.1078*pow(10,(7.5*tmp/(tmp+237.3))))/(tmp+273.15))/100;
      Serial.printf("Temperature: %2.2f*C  Humedad: %0.2f%%  Humidity Deficit: %0.2fg/m^2%\r\n", tmp, hum, housa);

      //Add the measured values to JSON
      api.add(tmp, sensor_tmp);
      api.add(hum, sensor_hum);
      api.add(housa, sensor_housa);
    }
    
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
    float res_tmp =  api.get(sensor_tmp);
    float res_hum =  api.get(sensor_hum);
    float res_housa =  api.get(sensor_housa);
    
    Serial.printf("Temperature: %2.2f*C  Humedad: %0.2f%%  Humidity Deficit: %0.2fg/m^2%(Received from the API)\r\n", res_tmp, res_hum, res_housa);
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}
