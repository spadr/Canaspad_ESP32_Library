#include "GoTrue.h"

GoTrue::GoTrue(const char* path, const int port) {
    this->backend_path = path;
    this->backend_port = port;
}

GoTrue::~GoTrue() {
    // TODO: sign out
    this->is_signed_in = false;
    this->access_token = "";
    this->refresh_token = "";
}

GoTrue& GoTrue::signIn(const char* host, const char* key, const char* username,
                       const char* password, const char* auth_endpoint) {
    if (host == nullptr || host[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: host is not set";
        return *this;
    }
    this->gotrue_host = host;

    if (key == nullptr || key[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: key is not set";
        return *this;
    }
    this->gotrue_key = key;

    if (auth_endpoint == nullptr || auth_endpoint[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: auth_endpoint is not set";
        return *this;
    }
    this->endpoint = auth_endpoint;

    if ((username != NULL && username[0]) && (password != NULL && password[0])) {
        this->email = username;
        this->password = password;
        this->is_signed_in = setAccessToken();
        return *this;
    }

    if ((username == nullptr || username[0] == 0) && (password == nullptr || password[0] == 0)) {
        return *this;
    }

    if (username == nullptr || username[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: username is not set";
    }

    if (password == nullptr || password[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: password is not set";
    }

    return *this;
}

String GoTrue::useAccessToken() {
    unsigned long elapsed = millis() - this->signed_in_at;
    Serial.println("elapsed: " + String(elapsed) + " expires_in: " + String(this->expires_in));
    if (elapsed > this->expires_in - expiration_offset) {
        bool keepAuth = refreshAccessToken() || setAccessToken();
    }
    // TODO : if session
    return this->access_token;
}

bool GoTrue::setAccessToken() {
    String send_message = "{\"email\":\"" + String(this->email) + "\",\"password\":\"" +
                          String(this->password) + "\"}";

    // send request to auth endpoint
    HttpClient client(String(this->gotrue_host), this->backend_port);
    client.setInsecure();
    client.setPath(String(this->backend_path) + String(this->endpoint));
    client.addParameter("grant_type", "password");
    client.addHeader("Host", String(this->gotrue_host));
    client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client.addHeader("Content-Type", "application/json");
    client.addHeader("apikey", String(this->gotrue_key));
    client.addHeader("Content-Length", String(send_message.length()));
    client.addHeader("Connection", "close");
    client.setBody(send_message);
    client.methodIsPost();
    HttpResponse* res_ptr = client.send();
    client.end();

    int status_code = res_ptr->checkStatusCode();
    if (status_code != 200) {
        this->error = true;
        this->error_message = "GoTrue: HTTP Request failed " + String(status_code) + " " +
                              String(res_ptr->checkReasonPhrase());
        return false;
    }


    StaticJsonDocument<2048> doc;
    DeserializationError deserialize_error = deserializeJson(doc, res_ptr->checkMessageBody());
    if (deserialize_error) {
        this->error = true;
        this->error_message =
            "GoTrue: deserializeJson() failed: " + String(deserialize_error.c_str());
        return false;
    }

    this->signed_in_at = millis();
    this->access_token = doc["access_token"].as<String>();
    this->refresh_token = doc["refresh_token"].as<String>();
    this->expires_in = doc["expires_in"].as<long>() * 1000;

    if (this->access_token == nullptr || this->access_token[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: failed to get access token";
        return false;
    }
    if (this->refresh_token == nullptr || this->refresh_token[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: failed to get refresh token";
        return false;
    }
    if (this->expires_in == 0) {
        this->error = true;
        this->error_message = "GoTrue: failed to get expires_in";
        return false;
    }

    this->is_signed_in = true;

    return true;
}

bool GoTrue::refreshAccessToken() {
    if (!this->is_signed_in) {
        this->error = true;
        this->error_message = "GoTrue: sign in first";
        return false;
    }

    String send_message = "{\"refresh_token\":\"" + String(this->refresh_token) + "\"}";

    // send request to auth endpoint
    HttpClient client(String(this->gotrue_host), this->backend_port);
    client.setInsecure();
    client.setPath(String(this->backend_path) + String(this->endpoint));
    client.addParameter("grant_type", "refresh_token");
    client.addHeader("Host", String(this->gotrue_host));
    client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client.addHeader("Content-Type", "application/json");
    client.addHeader("apikey", String(this->gotrue_key));
    client.addHeader("Content-Length", String(send_message.length()));
    client.addHeader("Connection", "close");
    client.setBody(send_message);
    client.methodIsPost();
    HttpResponse* res_ptr = client.send();
    client.end();

    int status_code = res_ptr->checkStatusCode();
    if (int(status_code / 100) != 2) {
        this->error = true;
        this->error_message = "GoTrue: HTTP Request failed " + String(status_code) + " " +
                              String(res_ptr->checkReasonPhrase());
        return false;
    }


    StaticJsonDocument<2048> doc;
    DeserializationError deserialize_error = deserializeJson(doc, res_ptr->checkMessageBody());
    if (deserialize_error) {
        this->error = true;
        this->error_message =
            "GoTrue: deserializeJson() failed: " + String(deserialize_error.c_str());
        return false;
    }

    this->signed_in_at = millis();
    this->access_token = doc["access_token"].as<String>();
    this->refresh_token = doc["refresh_token"].as<String>();
    this->expires_in = doc["expires_in"].as<long>() * 1000;

    if (this->access_token == nullptr || this->access_token[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: failed to get access token";
        return false;
    }
    if (this->refresh_token == nullptr || this->refresh_token[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: failed to get refresh token";
        return false;
    }
    if (this->expires_in == 0) {
        this->error = true;
        this->error_message = "GoTrue: failed to get expires_in";
        return false;
    }

    this->is_signed_in = true;

    return true;
}
