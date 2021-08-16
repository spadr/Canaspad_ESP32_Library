#include <M5StickC.h>
#include <Canaspad.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"


const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";


Canaspad api;
SHT3X sht30;
Adafruit_BMP280 bme;

String sensor_tmp;
String sensor_hum;
String sensor_press;
String sensor_housa;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;
float housa = 0.0;


void setup() {
  M5.begin();
  
  Serial.begin(115200);
  
  if(not api.begin(ssid, password, 9, api_username, api_password)){
    Serial.println("Api Connection Faild");
    Serial.println(api.httpCode);
  }
  
  Serial.print(F("ENV Unit(SHT30 and BMP280) test..."));
  while (!bme.begin(0x76)){  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  Serial.println("OK");
  
  //Get the token for each sensor
  sensor_tmp   = api.set("temp",  "env2", "number", 3, true);
  sensor_hum   = api.set("hum",   "env2", "number", 3, true);
  sensor_press = api.set("press", "env2", "number", 3, true);
  sensor_housa = api.set("housa", "env2", "number", 3, true);
}


void loop() {
  if (api.gettimestamp() % 60 == 0){//60-second interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Get the measured value of each sensor
    pressure = bme.readPressure()/100;
    if(sht30.get()==0){
      tmp = sht30.cTemp;
      hum = sht30.humidity;
    }
    housa = (100-hum)*(217*(6.1078*pow(10,(7.5*tmp/(tmp+237.3))))/(tmp+273.15))/100;
    
    Serial.printf("Temperature: %2.2f*C  Humedad: %0.2f%%\r\n", tmp, hum);
    Serial.printf("Pressure: %0.2fhPa  Humidity Deficit: %0.2fg/m^2%\r\n", pressure, housa);

    //Add the measured values to JSON
    api.add(String(tmp), sensor_tmp);
    api.add(String(hum), sensor_hum);
    api.add(String(pressure), sensor_press);
    api.add(String(housa), sensor_housa);

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
    float res_temp =  api.get(sensor_tmp);
    float res_hum =  api.get(sensor_hum);
    float res_press =  api.get(sensor_press);
    float res_housa =  api.get(sensor_housa);
    
    Serial.printf("Temperature: %2.2f*C  Humedad: %0.2f%%(Received from the API)\r\n", res_temp, res_hum);
    Serial.printf("Pressure: %0.2fhPa  Humidity Deficit: %0.2fg/m^2%(Received from the API)\r\n", res_press, res_housa);
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}
