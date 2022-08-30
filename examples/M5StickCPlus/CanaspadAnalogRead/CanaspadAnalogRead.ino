#include <M5StickCPlus.h>
#include <WiFiMulti.h>
#include <Canaspad.h>

const char *ssid = "WiFi_SSID";
const char *password = "WiFi_PASSWORD";
const char *api_url = "Canaspad_project_in_supabase";
const char *api_key = "anon_key";
const char *api_username = "user@mail.com";
const char *api_password = "password";
const char *ntp_server = "ntp.nict.jp";
const long  gmt_offset_sec     = 0;
const int   daylight_offset_sec = 3600 * 9;

Canaspad api(api_url, api_key, api_username, api_password);

float measured_value;
Tube voltage_sensor(&measured_value);

#define PIN 36
struct tm timeInfo;
http_code_t http_code;

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

    // Login to Canaspad API
    http_code = api.login();

    // TEST: Check HTTP status code
    if (http_code == "200")// TODO : use enum
    {
        Serial.println("Loggedin successfully!");
    }
    else
    {
        Serial.println("Failed to login!");
    }
    
    // Get the Tube token
    http_code = api.token("ch01", "name01", voltage_sensor);

    // TEST: Check HTTP status code
    if (http_code == "201")// TODO : use enum
    {
        Serial.println("Received Tube token successfully!");
    }
    else
    {
        Serial.println("Failed to receive Tube token!");
    }

    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
}

void loop()
{
    getLocalTime(&timeInfo);
    if (timeInfo.tm_sec == 0)
    { // 60-second interval
        Serial.println("---------------------------------------------");

        // Add the measured value to Tube object
        measured_value = (analogRead(PIN) + 1) * 3.3 * 1000 / (4095 + 1);
        Serial.printf("Voltage: %2.2fmV\r\n", measured_value);
        api.write(timeInfo, voltage_sensor);

        // TEST: Check if saved in Tube object
        if (voltage_sensor.saved_value_is(measured_value))
        {
            Serial.println("Saved successfully!");
        }
        else
        {
            Serial.println("Failed to save!");
        }

        // Send data to Canaspad API
        http_code = api.send(voltage_sensor);

        // TEST: Check HTTP status code
        if (http_code == "201")// TODO : use enum
        {
            Serial.println("Sent saved value successfully!");
        }
        else
        {
            Serial.println("Failed to send saved value!");
        }

        // Getting values from Canaspad API
        float fresh_value;
        api.fetch(&fresh_value, voltage_sensor);
        Serial.printf("Voltage: %2.2fmV(Received from the API)\r\n", fresh_value);

        // TEST: Check if saved in Canaspad API
        if (measured_value == fresh_value)
        {
            Serial.println("Synced successfully!");
        }
        else
        {
            Serial.println("Failed to sync!");
        }

        Serial.println("---------------------------------------------");
        Serial.println();

        delay(1000);
    }
}