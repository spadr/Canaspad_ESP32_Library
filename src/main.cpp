// #include "Config.h"
#include "ConfigExample.h"
#include <Canaspad.h>
#include <WiFiMulti.h>

Canaspad apiClient(api_host, api_username, api_password, gmt_offset_sec + daylight_offset_sec,
                   isrg_root_x1, client_cert, client_key);

float measured_value;
Tube voltage_sensor(&measured_value);

#define PIN 36
struct tm timeInfo;

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFiMulti wifiMulti;
    wifiMulti.addAP(ssid, password);
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(100);
        Serial.println("Connecting to WiFi..");
    }

    // Login to Canaspad API
    if (apiClient.login()) {
        Serial.println("Loggedin successfully!");
    } else {
        Serial.println("Failed to login!");
    }

    // Get the Tube token
    if (apiClient.token(voltage_sensor, "ch01", "name01")) {
        Serial.println("Received Tube token successfully!");
    } else {
        Serial.println("Failed to receive Tube token!");
    }

    configTime(gmt_offset_sec, daylight_offset_sec, ntp_host);
}

void loop() {
    getLocalTime(&timeInfo);
    if (timeInfo.tm_sec % 10 == 0) { // 60-second interval
        Serial.println("---------------------------------------------");

        // Add the measured value to Tube object
        // measured_value = (analogRead(PIN) + 1) * 3.3 * 1000 / (4095 + 1);
        measured_value = (random(0, 4095) + 1) * 3.3 * 1000 / (4095 + 1);

        Serial.printf("Voltage: %2.2fmV\r\n", measured_value);
        apiClient.write(voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                        timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
                        apiClient.offset_hour);

        // Check if saved in Tube object
        String now_tz = apiClient.makeTimestampTz(
            timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, timeInfo.tm_hour,
            timeInfo.tm_min, timeInfo.tm_sec, apiClient.offset_hour);
        String now =
            apiClient.makeTimestamp(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                                    timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);

        if (voltage_sensor.savedValueIs(measured_value) &&
            voltage_sensor.savedTimestampIs(now_tz)) {
            Serial.println("Saved successfully!");
        } else {
            Serial.println("Failed to save!");
        }

        // Send data to Canaspad API
        if (apiClient.send(voltage_sensor)) {
            Serial.println("Sent saved_value successfully!");
        } else {
            Serial.println("Failed to send!");
        }

        // Getting values from Canaspad API
        float fresh_value;
        String fresh_timestamp;
        bool fetched = apiClient.fetch(voltage_sensor, &fresh_value, &fresh_timestamp);
        Serial.printf("Voltage: %2.2fmV(Received from the API)\r\n", fresh_value);

        // Check if saved in Canaspad API
        if (fetched && now == fresh_timestamp && measured_value == fresh_value) {
            Serial.println("Synced successfully!");
        } else {
            Serial.println("Failed to sync!");
        }

        Serial.println("---------------------------------------------");
        Serial.println();

        delay(1000);
    }
}