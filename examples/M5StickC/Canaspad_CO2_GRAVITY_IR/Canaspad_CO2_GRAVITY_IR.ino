#include <M5StickC.h>
#include <Canaspad.h>

const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";
const int PIN            = 36;

Canaspad api;

String sensor_conc;

float conc = 0.0;


void setup() {
  M5.begin();
  
  Serial.begin(115200);
  
  if(not api.begin(ssid, password, 9, api_username, api_password)){
    Serial.println("Api Connection Faild");
    Serial.println(api.httpCode);
  }
  
  //Get the uuid
  sensor_conc   = api.set("conc",  "co2", "number");
}

void loop() {
  if (api.gettimestamp() % 60 == 0){//60-second interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Get the measured value
    float vol = (analogRead(PIN) + 1) * 3.3 / (4095 + 1);//0-3.3V,12bit(0-4095)
    conc  = 2900.63 * vol - 801.26;//Liner equation through P and Q. P(0.42v,417ppm), Q(2.0v,5000ppm)
    
    Serial.printf("CO2 Concentration: %0.2f%%\r\n", conc);

    //Add the measured values to JSON
    api.add(conc, sensor_conc);

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
    float res_conc =  api.get_float(sensor_conc);
    
    Serial.printf("CO2 Concentration: %0.2f%%(Received from the API)\r\n", res_conc);
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}
