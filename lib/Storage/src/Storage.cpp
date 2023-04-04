#include "Storage.h"

Storage::Storage(HttpClient** client_pp, const char* path, const int port) {
    this->backend_path = path;
    this->backend_port = port;
    this->client_pp = client_pp;
}


Storage::~Storage() {}

Storage& Storage::begin(const char* host) {
    this->backend_host = host;
    return *this;
}

Storage& Storage::begin(const char* host, GoTrue** gotrue_pp) {
    this->backend_host = host;
    this->gotrue_pp = gotrue_pp;
    this->use_token = true;

    GoTrue& auth_client = **gotrue_pp;
    HttpClient& http_client = **client_pp;
    String access_token = auth_client.useAccessToken();
    if (access_token == "") {
        this->error = true;
        this->error_message = "Storage: Access token is empty";
        return *this;
    }
    http_client.addHeader("Authorization", "Bearer " + access_token);
    return *this;
}

Storage& Storage::from(String path) {
    this->storage_path = path;
    return *this;
}

/*Storage& Storage::download(String file_name, uint8_t* pick_value, size_t* size) {
    this->file_name = file_name;
    HttpClient& http_client = **client_pp;
    http_client.methodIsGet();

    http_client.setPath(
        String(this->backend_path) +
        "/object/authenticated/image/userid/apple.png");
    http_client.addHeader("Host", String(this->backend_host));
    http_client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    http_client.addHeader("Connection", "close");

    // TODO : check if the client is ready to execute
    this->result = http_client.send();
    http_client.end();

    if (this->result.network_error) {
        this->error = true;
        this->error_message =
            "Storage: Network error is occured " + String(this->result.error_message);
        return *this;
    }

    int status_code = this->result.status_code;
    if (status_code == 200 || status_code == 201) {
        this->error = false;
        this->error_message = "";
        return *this;
    }

    this->error = true;
    this->error_message = "Storage: HTTP Request failed " + String(status_code) + " " +
                          String(this->result.reason_phrase);
    return *this;
}*/

Storage& Storage::upload(String file_name, ContentType content_type, uint8_t* payload,
                         size_t size) {
    this->file_name = file_name;
    HttpClient& http_client = **client_pp;
    http_client.methodIsPost();
    http_client.setFile(payload, size);
    http_client.addHeader("Host", String(this->backend_host));
    http_client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    http_client.addHeader("Content-Length", String(size));

    if (content_type == ContentType::JSON) {
        http_client.addHeader("Content-Type", "application/json");
    } else if (content_type == ContentType::CSV) {
        http_client.addHeader("Content-Type", "text/csv");
    } else if (content_type == ContentType::TEXT) {
        http_client.addHeader("Content-Type", "text/plain");
    } else if (content_type == ContentType::XML) {
        http_client.addHeader("Content-Type", "text/xml");
    } else if (content_type == ContentType::PNG) {
        http_client.addHeader("Content-Type", "image/png");
    } else if (content_type == ContentType::JPEG) {
        http_client.addHeader("Content-Type", "image/jpeg");
    } else if (content_type == ContentType::GIF) {
        http_client.addHeader("Content-Type", "image/gif");
    } else if (content_type == ContentType::SVG) {
        http_client.addHeader("Content-Type", "image/svg+xml");
    } else if (content_type == ContentType::PDF) {
        http_client.addHeader("Content-Type", "application/pdf");
    } else if (content_type == ContentType::ZIP) {
        http_client.addHeader("Content-Type", "application/zip");
    } else if (content_type == ContentType::MP3) {
        http_client.addHeader("Content-Type", "audio/mpeg");
    } else if (content_type == ContentType::MP4) {
        http_client.addHeader("Content-Type", "video/mp4");
    } else if (content_type == ContentType::OGG) {
        http_client.addHeader("Content-Type", "audio/ogg");
    } else if (content_type == ContentType::WAV) {
        http_client.addHeader("Content-Type", "audio/wav");
    } else if (content_type == ContentType::WEBM) {
        http_client.addHeader("Content-Type", "video/webm");
    } else {
        http_client.addHeader("Content-Type", "application/octet-stream");
    }

    http_client.addHeader("Connection", "close");
    return *this;
}

/*Storage& Storage::fix(String file_name, ContentType content_type, uint8_t* payload, size_t size) {
    return *this;
}*/

/*Storage& Storage::remove(String file_name) { return *this; }*/

Storage& Storage::execute() {
    HttpClient& http_client = **client_pp;
    http_client.setPath(String(this->backend_path) + this->storage_path + this->file_name);
    this->result = http_client.send();
    http_client.end();

    if (this->result.ok) {
        return *this;
    } else {
        if (this->result.network_error) {
            this->error = true;
            this->error_message =
                "Storage: Network error is occured " + String(this->result.error_message);
            return *this;
        }

        int status_code = this->result.status_code;
        if (status_code == 200 || status_code == 201) {
            this->error = false;
            this->error_message = "";
            return *this;
        }

        this->error = true;
        this->error_message = "Storage: HTTP Request failed " + String(status_code) + " " +
                              String(this->result.reason_phrase);
        return *this;
    }
}