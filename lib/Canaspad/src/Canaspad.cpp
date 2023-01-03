#include "Canaspad.h"

Canaspad::Canaspad(const char* host, const char* key, const char* username, const char* password,
                   const long offset_sec) {
    this->api_host = host;
    this->api_key = key;
    this->api_username = username;
    this->api_password = password;
    this->offset_hour = offset_sec / 3600;
}

Canaspad::~Canaspad() {
    // TODO: sign out
}

bool Canaspad::login() {
    GoTrue& auth_client = supabase.auth()->signIn(this->api_host, this->api_key, this->api_username,
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
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);
    db_client.from("tube").select("token").eq("channel", channel).eq("name", name).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }


    StaticJsonDocument<2048> doc;
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
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);
    db_client.from("tube").upsert(json).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }


    StaticJsonDocument<2048> doc;
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
    String _timestamp = sensor.timestamp();
    String json = sensor.elementParse();

    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);
    db_client.from("element").insert(json).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    return true;
}

void Canaspad::fetch(Tube& sensor, float* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);

    db_client.from("element")
        .select("value")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
    }


    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
    }


    if (doc[0] == nullptr) {
        this->error_message = "No element found";
    }

    float value = doc[0]["value"].as<float>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }
}

void Canaspad::fetch(Tube& sensor, int* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);

    db_client.from("element")
        .select("value")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
    }

    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
    }


    if (doc[0] == nullptr) {
        this->error_message = "No element found";
    }

    int value = doc[0]["value"].as<int>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }
}

void Canaspad::fetch(Tube& sensor, long* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);

    db_client.from("element")
        .select("value")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
    }

    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
    }


    if (doc[0] == nullptr) {
        this->error_message = "No element found";
    }

    long value = doc[0]["value"].as<long>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }
}

void Canaspad::fetch(Tube& sensor, unsigned int* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);

    db_client.from("element")
        .select("value")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
    }

    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
    }


    if (doc[0] == nullptr) {
        this->error_message = "No element found";
    }

    unsigned int value = doc[0]["value"].as<unsigned int>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }
}

void Canaspad::fetch(Tube& sensor, unsigned long* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue* auth_client = supabase.auth();
    PostgRest& db_client = supabase.rest()->begin(this->api_host, this->api_key, auth_client);

    db_client.from("element")
        .select("value")
        .eq("tube_token", sensor.checkToken())
        .order("created_at", false)
        .limit(1)
        .execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
    }

    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "json deserialize failed";
    }


    if (doc[0] == nullptr) {
        this->error_message = "No element found";
    }

    unsigned long value = doc[0]["value"].as<unsigned long>();
    *fresh_value_p = value;

    if (fresh_timestamp_p != nullptr) {
        String timestamp = doc[0]["created_at"].as<String>();
        *fresh_timestamp_p = timestamp;
    }
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