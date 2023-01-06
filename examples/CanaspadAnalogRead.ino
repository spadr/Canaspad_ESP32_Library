#include <Canaspad.h>
#include <WiFiMulti.h>

const char* ssid = "WiFi_SSID";
const char* password = "WiFi_PASSWORD";
const char* api_host = "canaspad_host";
const char* api_username = "user@mail.com";
const char* api_password = "password";
const char* ntp_host = "ntp.nict.jp";
const long gmt_offset_sec = 3600 * 9;
const int daylight_offset_sec = 0;
const char* isrg_root_x1 = "-----BEGIN CERTIFICATE-----\n" // Expiration: 2035-06-04 11:04:38 UTC
                           "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
                           "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
                           "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
                           "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
                           "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
                           "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
                           "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
                           "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
                           "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
                           "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
                           "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
                           "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
                           "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
                           "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
                           "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
                           "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
                           "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
                           "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
                           "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
                           "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
                           "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
                           "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
                           "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
                           "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
                           "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
                           "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
                           "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
                           "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
                           "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
                           "-----END CERTIFICATE-----\n";

Canaspad apiClient(api_host, api_username, api_password, gmt_offset_sec + daylight_offset_sec,
                   isrg_root_x1);

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
        measured_value = (analogRead(PIN) + 1) * 3.3 * 1000 / (4095 + 1);

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