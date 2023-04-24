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
    GoTrue** auth_client_pp = supabase.auth();
    GoTrue& auth_client = **auth_client_pp;
    auth_client.signIn(this->api_host, this->api_username, this->api_password,
                       canaspad_api_end_point.auth);
    if (auth_client.checkError()) {
        this->error_message = auth_client.checkErrorMessage();
        return false;
    }
    return auth_client.checkAuthStatus();
}

bool Canaspad::connect(Tube& sensor, String const channel, String const name) {
    // Get TUBE record if channel and name exist
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
    db_client.from("tube").select("token").eq("channel", channel).eq("name", name).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }


    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "Json deserialize failed";
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

bool Canaspad::createConnection(Tube& sensor, String const channel, String const name) {
    // TODO: Create TUBE record
    String json = "{\"channel\":\"" + channel + "\",\"name\":\"" + name + "\"}";
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;
    db_client.begin(this->api_host, auth_client_pp);
    db_client.from("tube").upsert(json).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No Tube token created";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "Json deserialize failed: " + String(error.c_str());
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
    String json = sensor.toJson();
    if (json == "") {
        this->error_message = "No data to send";
        return false;
    }

    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
    db_client.from("element").insert(json).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    return sensor.parsedElementIs(db_client.checkResult());
}

bool Canaspad::fetch(Tube& sensor, float* fresh_value_p, String* fresh_timestamp_p) {
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
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
        this->error_message = "Json deserialize failed";
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
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
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
        this->error_message = "Json deserialize failed";
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
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
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
        this->error_message = "Json deserialize failed";
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
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
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
        this->error_message = "Json deserialize failed";
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
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
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
        this->error_message = "Json deserialize failed";
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
    char buf[23];
    if (utc_offset_hour >= 0) {
        sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d+%02d", year, month, day, hour, minute, second,
                utc_offset_hour);
    } else {
        sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d-%02d", year, month, day, hour, minute, second,
                utc_offset_hour);
    }

    String timestamp = String(buf);
    return timestamp;
}

bool Canaspad::connect(Film& sensor, String const channel, String const name) {
    // Get TUBE record if channel and name exist
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
    db_client.from("film").select("token").eq("channel", channel).eq("name", name).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No token found";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "Json deserialize failed";
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

bool Canaspad::createConnection(Film& sensor, String const channel, String const name) {
    // TODO: Create TUBE record
    String json = "{\"channel\":\"" + channel + "\",\"name\":\"" + name + "\"}";
    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    PostgRest& db_client = **db_client_pp;
    db_client.begin(this->api_host, auth_client_pp);
    db_client.from("film").upsert(json).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No Film token created";
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, db_client.checkResult());
    if (error) {
        this->error_message = "Json deserialize failed: " + String(error.c_str());
        return false;
    }

    if (doc[0] != nullptr) {
        String token = doc[0]["token"].as<String>();
        sensor.begin(channel, name, token);
        return true;
    }
    return false;
}


bool Canaspad::write(Film& sensor, int year, int month, int day, int hour, int minute, int second,
                     int utc_offset_hour) {
    String timestamp_with_tz =
        makeTimestampTz(year, month, day, hour, minute, second, utc_offset_hour);
    return sensor.add(timestamp_with_tz);
}

bool Canaspad::send(Film& sensor) {
    String timestamp = sensor.timestamp();
    String new_molecule = sensor.toJson();
    if (new_molecule == "") {
        this->error_message = "No data to send";
        return false;
    }

    GoTrue** auth_client_pp = supabase.auth();
    PostgRest** db_client_pp = supabase.rest();
    Storage** storage_client_pp = supabase.storage();
    Storage& storage_client = **storage_client_pp;
    PostgRest& db_client = **db_client_pp;

    db_client.begin(this->api_host, auth_client_pp);
    db_client.from("molecule").insert(new_molecule).execute();

    if (db_client.checkError()) {
        this->error_message = db_client.checkErrorMessage();
        return false;
    }

    String created_molecule = db_client.checkResult();

    if (db_client.checkResult() == "" || db_client.checkResult() == "[]") {
        this->error_message = "No molecule created";
        return false;
    }

    DynamicJsonDocument deserialized_molecule(2048);
    DeserializationError error1 = deserializeJson(deserialized_molecule, created_molecule);
    if (error1) {
        this->error_message = "Json deserialize failed";
        return false;
    }
    if (deserialized_molecule[0] == nullptr) {
        this->error_message = "No molecule created";
        return false;
    }

    String rep_timestamp = deserialized_molecule[0]["created_at"].as<String>();
    timestamp.replace(" ", "T");
    int timezone_pos = timestamp.indexOf('+');
    if (timezone_pos == -1) {
        timezone_pos = timestamp.indexOf('-');
    }
    timestamp = timestamp.substring(0, timezone_pos);

    if (timestamp != rep_timestamp) {
        this->error_message = "Timestamp mismatch";
        return false;
    }

    String file_name = deserialized_molecule[0]["file_name"].as<String>();
    if (file_name == "") {
        this->error_message = "No file_name";
        return false;
    }

    String user_id = deserialized_molecule[0]["user_id"].as<String>();
    if (user_id == "") {
        this->error_message = "No user_id";
        return false;
    }

    String molecule_token = deserialized_molecule[0]["token"].as<String>();
    if (molecule_token == "") {
        this->error_message = "No molecule_token";
        return false;
    }

    uint8_t* payload;
    size_t size;
    sensor.value(&payload, &size);

    String save_dir = "/object/image/" + user_id + "/" + file_name;
    storage_client.begin(this->api_host, auth_client_pp);
    storage_client.upload(file_name, ContentType::JPEG, payload, size).to(save_dir).execute();

    if (storage_client.checkError()) {
        this->error_message = storage_client.checkErrorMessage();
        return false;
    }

    DynamicJsonDocument upload_path(2048);
    DeserializationError error2 = deserializeJson(upload_path, storage_client.checkResult());
    if (error2) {
        this->error_message = "Json deserialize failed: " + String(error2.c_str());
        return false;
    }

    String key = upload_path["Key"].as<String>();
    deserialized_molecule[0]["file_path"] = key;
    deserialized_molecule[0]["stored_in"] = storage_client.checkPath();
    deserialized_molecule[0]["is_delete"] = false;
    String saved_molecule;
    serializeJson(deserialized_molecule[0], saved_molecule);
    db_client.begin(this->api_host, auth_client_pp);
    db_client.from("molecule").eq("token", molecule_token).update(saved_molecule).execute();
    return true;
}

bool Canaspad::fetch(Film& sensor, uint8_t* fresh_value_p, String* fresh_timestamp_p) {
    return true;
}