#include <M5StickC.h>
#include <Canaspad.h>
#include "ADS1100.h"


const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";
const int PIN            = 36;

Canaspad api;
ADS1100 ads;

#define REF_VOL    3.3
#define ADC_BASE   REF_VOL/32768

String sensor_vol;

float vol = 0.0;


void setup() {
  M5.begin();
  
  Serial.begin(115200);
  
  if(not api.begin(ssid, password, 9, api_username, api_password)){
    Serial.println("Api Connection Faild");
    Serial.println(api.httpCode);
  }

  ads.getAddr_ADS1100(ADS1100_DEFAULT_ADDRESS);
  ads.setGain(GAIN_ONE);
  ads.setMode(MODE_CONTIN);
  ads.setRate(RATE_8);
  ads.setOSMode(OSMODE_SINGLE);
  ads.begin();
  
  //Get the token
  sensor_vol   = api.set("vol",  "analog", "number", 3, true);
}

void loop() {
  if (api.gettimestamp() % 60 == 0){//60-second interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Get the measured value
    vol = measure();
    
    Serial.printf("Voltage: %2.2fV\r\n", vol);

    //Add the measured values to JSON
    api.add(vol, sensor_vol);

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
    float res_vol =  api.get(sensor_vol);
    
    Serial.printf("Voltage: %2.2fV(Received from the API)\r\n", res_vol);
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}

float measure(){
    byte error;
    int8_t address;
    address = ads.ads_i2cAddress;
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t result;
        float temp,vol;
        Serial.println("Getting Differential Reading from ADS1100");
        Serial.println(" ");
        result = ads.Measure_Differential();
        Serial.print("Digital Value of Analog Input between Channel 0 and 1: ");
        Serial.println(result);
        temp = result * ADC_BASE * 4;
        vol = temp *1000;
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        return vol;
    }
    else
    {
        Serial.println("ADS1100 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
        return 1/0;
    }
}
