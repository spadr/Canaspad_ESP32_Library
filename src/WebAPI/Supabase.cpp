#include "Supabase.h"

Supabase::Supabase() {}

Supabase::~Supabase() {}

bool Supabase::begin(const char* host, const char* key, const char* username, const char* password,
                     const char* auth_endpoint) {
    this->supabase_host = host;
    if (key != NULL && key[0] != 0) {
        this->supabase_key = key;
    }

    if (username != NULL && username[0] != 0) {
        this->email = username;
        if (password != NULL && password[0] != 0) {
            this->password = password;
            this->useAccessToken = true;

            return setAccessToken(); // signin
        }
    }

    return true;
}

bool Supabase::setAccessToken() {
    if (!this->useAccessToken) {
        log_e("Supabase: username or password is not set");
        return false;
    }

    String send_message = "{\"email\":\"" + String(this->email) + "\",\"password\":\"" +
                          String(this->password) + "\"}";

    // send request to auth endpoint
    HttpClient client(String(this->supabase_host), 443);
    client.setInsecure();
    client.setPath(this->backend_auth_path + String("/token"));
    client.addParameter("grant_type", "password");
    client.addHeader("Host", String(this->supabase_host));
    client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client.addHeader("Content-Type", "application/json");
    client.addHeader("apikey", String(this->supabase_key));
    client.addHeader("Content-Length", String(send_message.length()));
    client.addHeader("Connection", "close");
    client.setBody(send_message);
    HttpResponse* res_ptr = client.post();
    Serial.println(res_ptr->checkMessageBody());


    if (this->response.checkStatusCode() != 200) {
        return false;
    }

    this->access_token = this->response.json_at("access_token").c_str();
    this->refresh_token = this->response.json_at("refresh_token").c_str();

    return true;
}

bool Supabase::refreshAccessToken() {
    if (!this->useAccessToken) {
        log_e("Supabase: username or password is not set");
        return false;
    }

    String send_message = "{\"refresh_token\":\"" + String(this->refresh_token) + "\"}";

    // send request to auth endpoint
    HttpClient client(String(this->supabase_host), 443);
    client.setInsecure();
    client.setPath(this->backend_auth_path + String("/token"));
    client.addParameter("grant_type", "password");
    client.addHeader("Host", String(this->supabase_host));
    client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client.addHeader("Content-Type", "application/json");
    client.addHeader("apikey", String(this->supabase_key));
    client.addHeader("Content-Length", String(send_message.length()));
    client.addHeader("Connection", "close");
    client.setBody(send_message);
    HttpResponse* res_ptr = client.post();
    Serial.println(res_ptr->checkHttpMessage());

    if (this->response.checkStatusCode() != 200) {
        return false;
    }

    this->refresh_token = this->response.json_at("refresh_token").c_str();

    return true;
}

bool Supabase::updateAccessToken() { return refreshAccessToken() || setAccessToken(); }

bool Supabase::deleteAccessToken() {
    this->useAccessToken = false;
    return true;
}

bool Supabase::from(char* table) {
    this->table = table;
    return true;
}

bool Supabase::select(char* column) {
    this->column = column;
    return true;
}

bool Supabase::insert(char* json) {
    this->json = json;
    return true;
}

bool Supabase::filter(char* query) {
    this->query = query;
    return true;
}

HttpResponse Supabase::execute(bool auth) {
    // send request

    if (response.checkStatusCode() == 201) {
        // TODO delete SQL query
        return response;
    }

    // TODO if HTTP400, refresh access token and retry
    if (response.checkStatusCode() == 400) {
        String error = this->response.json_at("error");
        if (error == "invalid_grant" && auth) {
            updateAccessToken();
        }
    }
    return response;
}