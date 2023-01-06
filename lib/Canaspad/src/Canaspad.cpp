#include "Canaspad.h"

Canaspad::Canaspad(const char* host, const char* username, const char* password,
                   const long offset_sec, const char* root_ca, const char* client_cert,
                   const char* client_key) {
    this->api_host = host;
    if (root_ca != nullptr) {
        this->supabase.setCACert(root_ca);
        if (client_cert != nullptr && client_key != nullptr) {
            this->supabase.setCertificate(client_cert);
            this->supabase.setPrivateKey(client_key);
        }
    } else {
        this->supabase.setInsecure();
    }

    this->api_username = username;
    this->api_password = password;
    this->offset_hour = offset_sec / 3600;
}

Canaspad::~Canaspad() {
    // TODO: sign out
}

bool Canaspad::login() {
    GoTrue& auth_client = supabase.auth()->signIn(this->api_host, this->api_username,
                                                  this->api_password, canaspad_api_end_point.auth);
    if (auth_client.checkError()) {
        this->error_message = auth_client.checkErrorMessage();
        return false;
    }
    return auth_client.checkAuthStatus();
}

bool Canaspad::token(Tube& sensor, String const channel, String const name) {
    // Get TUBE record if channel and name exist
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);
    db_client.from("tube").select("token").eq("channel", channel).eq("name", name).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }


    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
        return false;
    }


    if (doc[0] == nullptr) {
        this->error_message = "No token found";
        return false;
    }

    String token = doc[0]["token"].as<String>();
    sensor.begin(channel, name, token);
    return true;
}

bool Canaspad::createToken(Tube& sensor, String const channel, String const name) {
    // TODO: Create TUBE record
    String json = "{\"channel\":\"" + channel + "\",\"name\":\"" + name + "\"}";

    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);
    db_client.from("tube").upsert(json).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }


    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "deserializeJson failed: " + String(error.c_str());
        return false;
    }

    if (doc[0] != nullptr) {
        String token = doc[0]["token"].as<String>();
        sensor.begin(channel, name, token);
        return true;
    }
    return false;
}


bool Canaspad::write(Tube& sensor, int year, int month, int day, int hour, int minute, int second,
                     int utc_offset_hour) {
    String timestamp_with_tz =
        makeTimestampTz(year, month, day, hour, minute, second, utc_offset_hour);
    return sensor.add(timestamp_with_tz);
}

bool Canaspad::send(Tube& sensor) {
    // TODO: Post Tube record
    String timestamp = sensor.timestamp();
    String json = sensor.elementParse();

    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);
    db_client.from("element").insert(json).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    return sensor.parsedElementIs(db_client.checkResult());
}

bool Canaspad::fetch(Tube& sensor, float* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);

    db_client.from("element")
        .select("value, created_at")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No element found";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
        return false;
    }

    if (doc[0] == nullptr) {
        this->error_message = "No element found";
        return false;
    }


    float value = doc[0]["value"].as<float>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }

    return true;
}

bool Canaspad::fetch(Tube& sensor, int* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);

    db_client.from("element")
        .select("value, created_at")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No element found";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
        return false;
    }

    if (doc[0] == nullptr) {
        this->error_message = "No element found";
        return false;
    }

    int value = doc[0]["value"].as<int>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }

    return true;
}

bool Canaspad::fetch(Tube& sensor, long* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);

    db_client.from("element")
        .select("value, created_at")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No element found";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
        return false;
    }

    if (doc[0] == nullptr) {
        this->error_message = "No element found";
        return false;
    }

    long value = doc[0]["value"].as<long>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }

    return true;
}

bool Canaspad::fetch(Tube& sensor, unsigned int* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);

    db_client.from("element")
        .select("value, created_at")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No element found";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
        return false;
    }

    if (doc[0] == nullptr) {
        this->error_message = "No element found";
        return false;
    }

    unsigned int value = doc[0]["value"].as<unsigned int>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }

    return true;
}


bool Canaspad::fetch(Tube& sensor, unsigned long* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, auth_client);

    db_client.from("element")
        .select("value, created_at")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No element found";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
        return false;
    }

    if (doc[0] == nullptr) {
        this->error_message = "No element found";
        return false;
    }

    unsigned long value = doc[0]["value"].as<unsigned long>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }

    return true;
}

String Canaspad::makeTimestampTz(int year, int month, int day, int hour, int minute, int second,
                                 int utc_offset_hour) {
    // DONE: make timestamp
    char buf[23];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d+%02d", year, month, day, hour, minute, second,
            utc_offset_hour);
    String timestamp = String(buf);
    return timestamp;
}

String Canaspad::makeTimestamp(int year, int month, int day, int hour, int minute, int second) {
    char buf[20];
    sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d", year, month, day, hour, minute, second);
    String timestamp = String(buf);
    return timestamp;
}