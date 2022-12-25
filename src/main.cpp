#include <Canaspad.h>
// #include "Config.h"
#include "ConfigExample.h"
#include <WiFiMulti.h>

Canaspad api(api_host, api_key, api_username, api_password, gmt_offset_sec + daylight_offset_sec);

float measured_value;
Tube voltage_sensor(&measured_value);

#define PIN 36
struct tm timeInfo;

void setup() {
    Serial.begin(115200);

    WiFiMulti wifiMulti;
    wifiMulti.addAP(ssid, password);
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(100);
        Serial.println("Connecting to WiFi..");
    }

    // Login to Canaspad API
    if (api.login()) {
        Serial.println("Loggedin successfully!");
    } else {
        Serial.println("Failed to login!");
    }

    // Get the Tube token
    if (api.token("ch01", "name01", voltage_sensor)) {
        Serial.println("Received Tube token successfully!");
    } else {
        Serial.println(api.checkErrorMessage());
    }

    configTime(gmt_offset_sec, daylight_offset_sec, ntp_host);
}

void loop() {
    getLocalTime(&timeInfo);
    if (timeInfo.tm_sec % 60 == 0) { // 60-second interval
        Serial.println("---------------------------------------------");

        // Add the measured value to Tube object
        measured_value = (analogRead(PIN) + 1) * 3.3 * 1000 / (4095 + 1);
        Serial.printf("Voltage: %2.2fmV\r\n", measured_value);
        api.write(voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                  timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);

        // Check if saved in Tube object
        timestamp_tz_t now = api.makeTimestampTz(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                                                 timeInfo.tm_mday, timeInfo.tm_hour,
                                                 timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);
        if (voltage_sensor.savedValueIs(measured_value) && voltage_sensor.savedTimestampIs(now)) {
            Serial.println("Saved successfully!");
        } else {
            Serial.println("Failed to save!");
        }

        // Send data to Canaspad API
        if (api.send(voltage_sensor)) {
            Serial.println("Sent saved_value successfully!");
        } else {
            Serial.println(api.checkErrorMessage());
        }

        // Getting values from Canaspad API
        float fresh_value;
        api.fetch(&fresh_value, voltage_sensor);
        Serial.printf("Voltage: %2.2fmV(Received from the API)\r\n", fresh_value);

        // Check if saved in Canaspad API
        if (measured_value == fresh_value) {
            Serial.println("Synced successfully!");
        } else {
            Serial.println("Failed to sync!");
        }

        Serial.println("---------------------------------------------");
        Serial.println();

        delay(1000);
    }
}