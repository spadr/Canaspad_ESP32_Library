#include <WiFiMulti.h>
#include "Canaspad.h"
#include "ConfigExample.h"
// #include "Config.h"

Canaspad api(api_url, api_key, api_username, api_password, gmt_offset_sec + daylight_offset_sec);

float measured_value;
Tube voltage_sensor(&measured_value);

#define PIN 36
struct tm timeInfo;

void setup()
{
    Serial.begin(115200);

    WiFiMulti wifiMulti;
    wifiMulti.addAP(ssid, password);
    while (wifiMulti.run() != WL_CONNECTED)
    {
        delay(100);
        Serial.println("Connecting to WiFi..");
    }

    // Login to Canaspad API
    if (api.login() == 200) // Check HTTP status code
    {
        Serial.println("Loggedin successfully!");
    }
    else
    {
        Serial.println("Failed to login!");
    }

    // Get the Tube token
    if (api.token("ch01", "name01", voltage_sensor) == 201) // Check HTTP status code
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
    if (timeInfo.tm_sec % 10 == 0)
    { // 60-second interval
        Serial.println("---------------------------------------------");

        // Add the measured value to Tube object
        measured_value = (analogRead(PIN) + 1) * 3.3 * 1000 / (4095 + 1);
        Serial.printf("Voltage: %2.2fmV\r\n", measured_value);
        api.write(voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);

        // Check if saved in Tube object
        timestamp_tz_t now = api._make_timestamp_tz(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);
        if (voltage_sensor.saved_value_is(measured_value) && voltage_sensor.saved_timestamp_is(now))
        {
            Serial.println("Saved successfully!");
        }
        else
        {
            Serial.println("Failed to save!");
        }

        // Send data to Canaspad API
        if (api.send(voltage_sensor) == 201) // Check HTTP status code
        {
            Serial.println("Sent saved_value successfully!");
        }
        else
        {
            Serial.println("Failed to send saved_value!");
        }

        // Getting values from Canaspad API
        float fresh_value;
        api.fetch(&fresh_value, voltage_sensor);
        Serial.printf("Voltage: %2.2fmV(Received from the API)\r\n", fresh_value);

        // Check if saved in Canaspad API
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