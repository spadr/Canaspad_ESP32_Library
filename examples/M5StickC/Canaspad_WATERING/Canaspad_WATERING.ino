#include <M5StickC.h>
#include <Canaspad.h>

#define INPUT_PIN 33
#define PUMP_PIN 32


const char  ssid[]       = "WiFi_ssid";
const char  password[]   = "WiFi_pass";
const char* api_username = "user@mail.com";
const char* api_password = "password";


Canaspad api;

String sensor_moisture;

float vol, moisture, threshold_moisture;


void setup() {
  M5.begin();
  
  Serial.begin(115200);
  
  if(not api.begin(ssid, password, 9, api_username, api_password)){
    Serial.println("Api Connection Faild");
    Serial.println(api.httpCode);
  }

  pinMode(INPUT_PIN, INPUT);
  pinMode(PUMP_PIN,OUTPUT);
  
  //Get the uuid
  sensor_moisture   = api.set("moisture",  "watering", "number", 3, true);
  //control_threshold   = api.set("moisture",  "control", "number", 3, true);//You can also fetch the thresholds recorded in the cloud.
  
}

void loop() {
  if (api.gettimestamp() % 60*60 == 0){// interval
    Serial.println();
    Serial.println("---------------------------------------------");

    //Get the measured value
    float adc =  analogRead(INPUT_PIN);
          adc += analogRead(INPUT_PIN);
          adc += analogRead(INPUT_PIN);
          adc += analogRead(INPUT_PIN);
          adc += analogRead(INPUT_PIN);
          adc = adc / 5;
    
    vol = (adc + 1) * 3.3 / (4095 + 1);//0-3.3V,12bit(0-4095)
    moisture  = 100 * (1.6 - vol) / (1.6 - 1.2);//100 * (dry_voltage - now_voltage) / (dry_voltage - full_of_water_voltage)
    
    Serial.printf("Voltage: %2.2fV  Moisture: %0.2f%%\r\n", vol, moisture);

    //Add the measured values to JSON
    api.add(moisture, sensor_moisture);

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
    float res_moisture =  api.get(sensor_moisture);
    
    Serial.printf("Moisture: %0.2f%%(Received from the API)\r\n", res_moisture);

    //Judge the watering
    threshold_moisture = 50.0;//Turn on when 50% or less
    //threshold_moisture = api.get(control_threshold);//You can also fetch the thresholds recorded in the cloud.
    
    if (moisture <= threshold_moisture){
      Serial.println("Needs watering");
      
      digitalWrite(PUMP_PIN,HIGH);
      delay(1000*30);//pump discharge:8.3 ml/s (without height difference)
      digitalWrite(PUMP_PIN,LOW);
    }
    
    Serial.println("---------------------------------------------");
    delay(10*1000);
  }
}
