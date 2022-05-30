#include <M5StickC.h>
#include <Canaspad.h>
#include "ADS1100.h"



const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";


Canaspad api;
ADS1100 ads;

#define REF_VOL    3.3
#define ADC_BASE   REF_VOL/32768

String sensor_vol;

float vol = 0.0;


void setup() {
  M5.begin();
  
  Serial.begin(115200);
  
  //api.domain("own.server.com");//Sending to your own server
  
  api.wifi(ssid, password);//Add WiFi configuration
  
  if(not api.begin(api_username, api_password, 9)){
    Serial.println("Connection Faild");
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
    //Get the measured value
    Serial.println();
    Serial.println("---------------------------------------------");
    int8_t address;
    address = ads.ads_i2cAddress;
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    
    if (error == 0){
      int16_t result = ads.Measure_Differential();
      float vol = result * ADC_BASE * 4 * 1000;
      Serial.printf("Voltage: %2.2fmV\r\n", vol);
    }else{
      String vol = "nan";
      Serial.println("ADS1100 Disconnected!");
    }

    
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
    
    Serial.printf("Voltage: %2.2fmV(Received from the API)\r\n", res_vol);
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}
