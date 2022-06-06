#include <M5StickCPlus.h>
#include <Canaspad.h>

const char* ssid         = "WiFi_ssid";
const char* password     = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";
const int PIN            = 36;

Canaspad api;

String sensor_vol;

float vol = 0.0;


void setup() {
  M5.begin();
  
  Serial.begin(115200);
  
  //api.domain("own.server.com");//Sending to your own server
  
  api.wifi(ssid, password);//Add WiFi configuration
  //api.wifi("ssid2", "password2");//Add multiple WiFi settings.
  
  if(not api.begin(api_username, api_password)){
    Serial.println("Connection Faild");
  }
  
  //Get the token
  sensor_vol   = api.set("vol",  "analog");
}

void loop() {
  if (api.gettimestamp() % 60 == 0){//60-second interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Get the measured value
    vol = (analogRead(PIN) + 1) * 3.3 *1000 / (4095 + 1);
    
    Serial.printf("Voltage: %2.2fmV\r\n", vol);

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
