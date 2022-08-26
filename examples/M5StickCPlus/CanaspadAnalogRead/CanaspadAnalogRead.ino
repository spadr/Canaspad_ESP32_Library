#include <M5StickCPlus.h>
#include <WiFiMulti.h>
#include <Canaspad.h>

const char *ssid = "WiFi_SSID";
const char *password = "WiFi_PASSWORD";
const char *api_url = "Canaspad_project_in_supabase";
const char *api_key = "anon_key";
const char *api_username = "user@mail.com";
const char *api_password = "password";
const char* ntp_server = "ntp.nict.jp";
const long gmt_offset_sec     = 0;
const int daylight_offset_sec = 3600 * 9;


Canaspad api(api_url, api_key, api_username, api_password);

float sensing_value;
Tube voltage_sensor(&sensing_value);

#define PIN 36
struct tm timeInfo;

void setup()
{
    M5.begin();

    Serial.begin(115200);
    
    WiFiMulti wifiMulti;
    wifiMulti.addAP(ssid, password);
    while (wifiMulti.run() != WL_CONNECTED)
    {
        delay(100);
        Serial.println("Connecting to WiFi..");
    }

    if (not api.begin())
    {
        Serial.println("Connection Faild");
        while (1)
        {
        };
    }

    // Get the Tube token
    api.set("ch01", "name01", voltage_sensor);

    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
}

void loop()
{
    getLocalTime(&timeInfo);
    if (timeInfo.tm_sec%10 == 0)
    { // 60-second interval
        Serial.println();
        Serial.println("---------------------------------------------");

        // Get the measured value
        sensing_value = (analogRead(PIN) + 1) * 3.3 * 1000 / (4095 + 1);

        Serial.printf("Voltage: %2.2fmV\r\n", sensing_value);

        // Add the measured values to Tube object
        api.write(timeInfo, voltage_sensor);

        // Send data to API
        http_code_t http_code = api.send(voltage_sensor);
        if (http_code == "201") // TODO : use enum
        {
            Serial.println("HTTP request succeeded!");
        }
        else
        {
            Serial.print("Error on HTTP request! HttpCode : ");
            Serial.println(http_code);
        }

        // Getting values from API
        float fresh_value;
        api.fetch(&fresh_value, voltage_sensor);

        Serial.printf("Voltage: %2.2fmV(Received from the API)\r\n", fresh_value);
        Serial.println("---------------------------------------------");

        delay(1000);
    }
}
