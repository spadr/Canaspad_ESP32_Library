// Using the Canaspad library to read data from M5StackTimerCameraX and send it to the Canaspad API.

#include "ConfigExample.h"
#include "camera_pins.h"
#include "esp_camera.h"
#include <Canaspad.h>
#include <WiFiMulti.h>

#define CAMERA_MODEL_M5STACK_PSRAM

Canaspad apiClient(api_host, api_username, api_password, gmt_offset_sec + daylight_offset_sec,
                   isrg_root_x1, client_cert, client_key);

uint8_t* buffer;
size_t buffer_size;
Film image_sensor(&buffer, &buffer_size);

#define PIN 36
struct tm timeInfo;

void setup() {
    Serial.begin(115200);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 10000000;
    config.frame_size = FRAMESIZE_QXGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 3;
    config.fb_count = 1;

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

    // Get the Film token
    if (apiClient.token(image_sensor, "ch01", "name01")) {
        Serial.println("Received Film token successfully!");
    } else {
        Serial.println("Failed to receive Tube token!");
    }

    configTime(gmt_offset_sec, daylight_offset_sec, ntp_host);

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
    }
}

void loop() {
    getLocalTime(&timeInfo);
    if (timeInfo.tm_min % 5 == 0) { // 5min interval
        camera_fb_t* fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
        } else {
            Serial.println("Camera capture success");
            buffer = fb->buf;
            buffer_size = fb->len;
            apiClient.write(image_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                            timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
                            apiClient.offset_hour);
            // Send data to Canaspad API
            if (apiClient.send(image_sensor)) {
                Serial.println("Sent successfully!");
            } else {
                Serial.println("Failed to send!");
            }
            buffer = nullptr;
            buffer_size = 0;
        }
        esp_camera_fb_return(fb);

        Serial.println("---------------------------------------------");
        Serial.println();

        delay(60 * 1000);
    }
}