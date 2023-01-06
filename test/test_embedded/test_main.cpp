#include "Config.h"
#include <Canaspad.h>
// #include "ConfigExample.h"
#include <EEPROM.h>
#include <WiFiMulti.h>
#include <unity.h>

Canaspad apiClient(api_host, api_username, api_password, gmt_offset_sec + daylight_offset_sec,
                   isrg_root_x1, client_cert, client_key);

bool is_stack_overflow;

float float_measured_value;
Tube float_voltage_sensor(&float_measured_value);

int int_measured_value;
Tube int_voltage_sensor(&int_measured_value);

long long_measured_value;
Tube long_voltage_sensor(&long_measured_value);

unsigned int uint_measured_value;
Tube uint_voltage_sensor(&uint_measured_value);

unsigned long ulong_measured_value;
Tube ulong_voltage_sensor(&ulong_measured_value);

struct tm timeInfo;

void test_stack_overflow() {
    EEPROM.begin(2);
    EEPROM.get(0, is_stack_overflow);
    if (!is_stack_overflow) {
        EEPROM.put(0, true);
        EEPROM.commit();
    }
    TEST_ASSERT_FALSE(is_stack_overflow);
}

void test_stack_overflow_() {
    EEPROM.begin(2);
    EEPROM.put(0, false);
    EEPROM.commit();
    EEPROM.get(0, is_stack_overflow);
    TEST_ASSERT_FALSE(is_stack_overflow);
}

void test_wifi_connection() {
    WiFiMulti wifiMulti;
    wifiMulti.addAP(ssid, password);

    int timeout = 100;
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(100);
        timeout -= 1;
        if (timeout == 0) {
            break;
        }
    }
    TEST_ASSERT_TRUE(wifiMulti.run() == WL_CONNECTED);
}

void test_login_to_server() { TEST_ASSERT_TRUE(apiClient.login()); }

void test_sync_float_tube_token() {
    TEST_ASSERT_TRUE(apiClient.token(float_voltage_sensor, "float_ch_01", "float_name_01"));
}

void test_sync_int_tube_token() {
    TEST_ASSERT_TRUE(apiClient.token(int_voltage_sensor, "int_ch_01", "int_name_01"));
}

void test_sync_long_tube_token() {
    TEST_ASSERT_TRUE(apiClient.token(long_voltage_sensor, "long_ch_01", "long_name_01"));
}

void test_sync_uint_tube_token() {
    TEST_ASSERT_TRUE(apiClient.token(uint_voltage_sensor, "uint_ch_01", "uint_name_01"));
}

void test_sync_ulong_tube_token() {
    TEST_ASSERT_TRUE(apiClient.token(ulong_voltage_sensor, "ulong_ch_01", "ulong_name_01"));
}

void test_sync_float_tube_value() {
    long _random_number = random(100000, 999999);
    float random_number = (float)_random_number / 1000.0;
    random_number += random_number / 1000000.0;
    if (random(0, 2)) {
        random_number = random_number * -1;
    }
    float_measured_value = random_number;
    getLocalTime(&timeInfo);
    String now =
        apiClient.makeTimestamp(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    apiClient.write(float_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                    timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
                    apiClient.offset_hour);
    if (!apiClient.send(float_voltage_sensor)) {
        return;
    }
    float fresh_value;
    String fresh_timestamp;
    if (!apiClient.fetch(float_voltage_sensor, &fresh_value)) {
        return;
    }
    if (fresh_timestamp != now) {
        return;
    }
    TEST_ASSERT_EQUAL_FLOAT(random_number, fresh_value);
}

void test_sync_int_tube_value() {
    long _random_number = random(32700, 32767);
    int random_number = _random_number;
    if (random(0, 2)) {
        random_number = random_number * -1;
    }
    int_measured_value = random_number;
    getLocalTime(&timeInfo);
    String now =
        apiClient.makeTimestamp(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    apiClient.write(int_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                    timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
                    apiClient.offset_hour);
    if (!apiClient.send(int_voltage_sensor)) {
        return;
    }
    int fresh_value;
    String fresh_timestamp;
    if (!apiClient.fetch(int_voltage_sensor, &fresh_value)) {
        return;
    }
    if (fresh_timestamp != now) {
        return;
    }
    TEST_ASSERT_EQUAL_INT16(random_number, fresh_value);
}

void test_sync_long_tube_value() {
    long random_number = random(2147483600, 2147483647);
    if (random(0, 2)) {
        random_number = random_number * -1;
    }
    int_measured_value = random_number;
    getLocalTime(&timeInfo);
    String now =
        apiClient.makeTimestamp(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    apiClient.write(int_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                    timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
                    apiClient.offset_hour);
    if (!apiClient.send(int_voltage_sensor)) {
        return;
    }
    long fresh_value;
    String fresh_timestamp;
    if (!apiClient.fetch(int_voltage_sensor, &fresh_value)) {
        return;
    }
    if (fresh_timestamp != now) {
        return;
    }
    TEST_ASSERT_EQUAL_INT32(random_number, fresh_value);
}

void test_sync_uint_tube_value() {
    long _random_number = random(65500, 65535);
    unsigned int random_number = _random_number;
    uint_measured_value = random_number;
    getLocalTime(&timeInfo);
    String now =
        apiClient.makeTimestamp(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    apiClient.write(uint_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                    timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
                    apiClient.offset_hour);
    if (!apiClient.send(uint_voltage_sensor)) {
        return;
    }
    unsigned int fresh_value;
    String fresh_timestamp;
    if (!apiClient.fetch(uint_voltage_sensor, &fresh_value)) {
        return;
    }
    if (fresh_timestamp != now) {
        return;
    }
    TEST_ASSERT_EQUAL_UINT16(random_number, fresh_value);
}

void test_sync_ulong_tube_value() {
    long _random_number = random(2147483600, 2147483647);
    unsigned long random_number = _random_number * 2;
    ulong_measured_value = random_number;
    getLocalTime(&timeInfo);
    String now =
        apiClient.makeTimestamp(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    apiClient.write(ulong_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1,
                    timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
                    apiClient.offset_hour);
    if (!apiClient.send(ulong_voltage_sensor)) {
        return;
    }
    unsigned long fresh_value;
    String fresh_timestamp;
    if (!apiClient.fetch(ulong_voltage_sensor, &fresh_value, &fresh_timestamp)) {
        return;
    }
    if (fresh_timestamp != now) {
        return;
    }
    TEST_ASSERT_EQUAL_UINT32(random_number, fresh_value);
}


void setup() {
    UNITY_BEGIN();
    delay(500);

    RUN_TEST(test_stack_overflow);
    RUN_TEST(test_wifi_connection);
    RUN_TEST(test_login_to_server);
    RUN_TEST(test_sync_float_tube_token);
    RUN_TEST(test_sync_int_tube_token);
    RUN_TEST(test_sync_long_tube_token);
    RUN_TEST(test_sync_uint_tube_token);
    RUN_TEST(test_sync_ulong_tube_token);

    configTime(gmt_offset_sec, daylight_offset_sec, ntp_host);

    for (int i = 0; i < 5; i++) {
        RUN_TEST(test_sync_float_tube_value);
        RUN_TEST(test_sync_int_tube_value);
        RUN_TEST(test_sync_long_tube_value);
        RUN_TEST(test_sync_uint_tube_value);
        RUN_TEST(test_sync_ulong_tube_value);
    }

    RUN_TEST(test_stack_overflow_);
    UNITY_END();
}

void loop() {}