#include "Config.h"
#include <Canaspad.h>
// #include "ConfigExample.h"
#include <WiFiMulti.h>


#include <unity.h>

Canaspad api(api_host, api_key, api_username, api_password, gmt_offset_sec + daylight_offset_sec);

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

void test_wifi() {
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

void test_login() { TEST_ASSERT_TRUE(api.login()); }

void test_sync_float_tube_token() {
    TEST_ASSERT_TRUE(api.token("float_ch_01", "float_name_01", float_voltage_sensor));
}

void test_sync_int_tube_token() {
    TEST_ASSERT_TRUE(api.token("int_ch_01", "int_name_01", int_voltage_sensor));
}

void test_sync_long_tube_token() {
    TEST_ASSERT_TRUE(api.token("long_ch_01", "long_name_01", long_voltage_sensor));
}

void test_sync_uint_tube_token() {
    TEST_ASSERT_TRUE(api.token("uint_ch_01", "uint_name_01", uint_voltage_sensor));
}

void test_sync_ulong_tube_token() {
    TEST_ASSERT_TRUE(api.token("ulong_ch_01", "ulong_name_01", ulong_voltage_sensor));
}

void test_sync_float_tube_value() {
    long _random_number = random(100000, 999999);
    float random_number = _random_number / 1000.0;
    if (random(0, 1)) {
        random_number = random_number * -1;
    }
    float_measured_value = random_number;
    getLocalTime(&timeInfo);
    api.write(float_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
              timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);
    api.send(float_voltage_sensor);
    float fresh_value;
    api.fetch(&fresh_value, float_voltage_sensor);
    TEST_ASSERT_EQUAL_FLOAT(random_number, fresh_value);
}

void test_sync_int_tube_value() {
    long _random_number = random(32700, 32767);
    int random_number = _random_number;
    if (random(0, 1)) {
        random_number = random_number * -1;
    }
    int_measured_value = random_number;
    getLocalTime(&timeInfo);
    api.write(int_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
              timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);
    api.send(int_voltage_sensor);
    int fresh_value;
    api.fetch(&fresh_value, int_voltage_sensor);
    TEST_ASSERT_EQUAL_INT16(random_number, fresh_value);
}

void test_sync_long_tube_value() {
    long random_number = random(2147483600, 2147483647);
    if (random(0, 1)) {
        random_number = random_number * -1;
    }
    int_measured_value = random_number;
    getLocalTime(&timeInfo);
    api.write(int_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
              timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);
    api.send(int_voltage_sensor);
    int fresh_value;
    api.fetch(&fresh_value, int_voltage_sensor);
    TEST_ASSERT_EQUAL_INT32(random_number, fresh_value);
}

void test_sync_uint_tube_value() {
    long _random_number = random(65500, 65535);
    int random_number = _random_number;
    uint_measured_value = random_number;
    getLocalTime(&timeInfo);
    api.write(uint_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
              timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);
    api.send(uint_voltage_sensor);
    unsigned int fresh_value;
    api.fetch(&fresh_value, uint_voltage_sensor);
    TEST_ASSERT_EQUAL_UINT16(random_number, fresh_value);
}

void test_sync_ulong_tube_value() {
    long _random_number = random(2147483600, 2147483647);
    unsigned long random_number = _random_number * 2;
    ulong_measured_value = random_number;
    getLocalTime(&timeInfo);
    api.write(ulong_voltage_sensor, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
              timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, api.offset_hour);
    api.send(ulong_voltage_sensor);
    unsigned long fresh_value;
    api.fetch(&fresh_value, ulong_voltage_sensor);
    TEST_ASSERT_EQUAL_UINT32(random_number, fresh_value);
}


void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_wifi);
    RUN_TEST(test_login);
    RUN_TEST(test_sync_float_tube_token);
    RUN_TEST(test_sync_int_tube_token);
    RUN_TEST(test_sync_long_tube_token);
    RUN_TEST(test_sync_uint_tube_token);
    RUN_TEST(test_sync_ulong_tube_token);
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_host);
    RUN_TEST(test_sync_float_tube_value);
    RUN_TEST(test_sync_int_tube_value);
    RUN_TEST(test_sync_long_tube_value);
    RUN_TEST(test_sync_uint_tube_value);
    RUN_TEST(test_sync_ulong_tube_value);
    UNITY_END();
}

void loop() {}