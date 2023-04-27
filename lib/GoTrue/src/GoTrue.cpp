#include "GoTrue.h"

GoTrue::GoTrue(HttpClient** client_pp, const char* path, const int port)
    : backend_path(path), backend_port(port), client_pp(client_pp) {}

GoTrue::~GoTrue() {
    // TODO: sign out
    this->is_signed_in = false;
    this->access_token = "";
    this->refresh_token = "";
}

GoTrue& GoTrue::signIn(const char* host, const char* username, const char* password,
                       const char* auth_endpoint) {
    if (host == nullptr || host[0] == 0) {
        this->error = true;
        this->error_message = "GoTrue: host is not set";
        return *this;
    }
    this->gotrue_host = host;

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
    if (elapsed > this->expires_in - expiration_offset) {
        bool keepAuth = refreshAccessToken() || setAccessToken();
        if (!keepAuth) {
            this->is_signed_in = false;
            this->access_token = "";
            this->refresh_token = "";
            this->error = true;
            this->error_message = "GoTrue: access token is expired";
        }
    }
    // TODO : if session
    return this->access_token;
}

bool GoTrue::setAccessToken() {
    HttpClient& client = **this->client_pp;
    String send_message = "{\"email\":\"" + String(this->email) + "\",\"password\":\"" +
                          String(this->password) + "\"}";

    // send request to auth endpoint
    client.setHost(String(this->gotrue_host));
    client.setPort(this->backend_port);
    client.setPath(String(this->backend_path) + String(this->endpoint));
    client.addParameter("grant_type", "password");
    client.addHeader("Host", String(this->gotrue_host));
    client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Content-Length", String(send_message.length()));
    client.addHeader("Connection", "close");
    client.setBody(send_message);
    client.methodIsPost();
    this->result = client.send();
    client.end();

    if (!this->result.ok) {
        if (this->result.network_error) {
            this->error = true;
            this->error_message =
                "GoTrue: Network error is occured" + String(this->result.error_message);
            return false;
        }

        int status_code = this->result.status_code;
        if (status_code != 200) {
            this->error = true;
            this->error_message = "GoTrue: HTTP Request failed " + String(status_code) + " " +
                                  String(this->result.reason_phrase);
            return false;
        }
    }


    DynamicJsonDocument doc(2048);
    DeserializationError deserialize_error = deserializeJson(doc, this->result.message_body);
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
    HttpClient& client = **this->client_pp;
    client.setHost(String(this->gotrue_host));
    client.setPort(this->backend_port);
    client.setPath(String(this->backend_path) + String(this->endpoint));
    client.addParameter("grant_type", "refresh_token");
    client.addHeader("Host", String(this->gotrue_host));
    client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Content-Length", String(send_message.length()));
    client.addHeader("Connection", "close");
    client.setBody(send_message);
    client.methodIsPost();
    this->result = client.send();
    client.end();

    int status_code = this->result.status_code;
    if (int(status_code / 100) != 2) {
        this->error = true;
        this->error_message = "GoTrue: HTTP Request failed " + String(status_code) + " " +
                              String(this->result.reason_phrase);
        return false;
    }


    DynamicJsonDocument doc(2048);
    DeserializationError deserialize_error = deserializeJson(doc, this->result.message_body);
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
