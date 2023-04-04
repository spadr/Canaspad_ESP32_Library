#include "HttpRequest.h"


HttpRequest::HttpRequest(HttpResponse* response_ptr) { this->response_ptr = response_ptr; }

HttpRequest::~HttpRequest() {}

bool HttpRequest::setCACert(const char* root_ca) {
    this->client_ptr->setCACert(root_ca);
    return true;
}

bool HttpRequest::setCertificate(const char* client_cert) {
    this->client_ptr->setCertificate(client_cert);
    return true;
}

bool HttpRequest::setPrivateKey(const char* client_key) {
    this->client_ptr->setPrivateKey(client_key);
    return true;
}

bool HttpRequest::setInsecure() {
    this->client_ptr->setInsecure();
    return true;
}

bool HttpRequest::setHost(String host) {
    this->host = host;
    return true;
}

bool HttpRequest::setPort(unsigned int port) {
    this->port = port;
    return true;
}

bool HttpRequest::setPath(String path) {
    this->path = path;
    return true;
}


bool HttpRequest::addParameter(String key, String value) {
    if (this->params == "?") {
        this->params += key + "=" + value;
    } else {
        this->params += "&" + key + "=" + value;
    }
    return true;
}

bool HttpRequest::addHeader(String key, String value) {
    this->header += key + ": " + value + "\r\n";
    return true;
}

bool HttpRequest::setBody(String body) {
    this->body = body;
    return true;
}

bool HttpRequest::setFile(uint8_t* data, size_t size) {
    this->file_data = data;
    this->file_size = size;
    this->file_mode = true;
    return true;
}

bool HttpRequest::methodIsGet() {
    this->request_line = "GET";
    return true;
}

bool HttpRequest::methodIsPost() {
    this->request_line = "POST";
    return true;
}

bool HttpRequest::methodIsPut() {
    this->request_line = "PUT";
    return true;
}

bool HttpRequest::methodIsPatch() {
    this->request_line = "PATCH";
    return true;
}

bool HttpRequest::methodIsDelete() {
    this->request_line = "DELETE";
    return true;
}

bool HttpRequest::methodIsHead() {
    this->request_line = "HEAD";
    return true;
}

Result HttpRequest::send() {
    Result result;

    if (this->host == "") {
        result.network_error = true;
        result.error_message = "Host is not set";
        return result;
    }

    if (this->port == 0) {
        result.network_error = true;
        result.error_message = "Port is not set";
        return result;
    }

    if (!this->client_ptr->connect(this->host.c_str(), this->port)) {
        result.network_error = true;
        result.error_message = "Connection failed";
        return result;
    }

    if (!this->client_ptr->connected()) {
        result.network_error = true;
        result.error_message = "Connection lost";
        return result;
    }

    String request_target;
    if (this->params == "?") {
        request_target = this->path;
    } else {
        request_target = this->path + this->params;
    }

    this->request_line += " " + request_target + " HTTP/1.1\r\n";

    this->client_ptr->print(this->request_line);
    this->client_ptr->print(this->header);
    this->client_ptr->print("\r\n");

    if (this->file_mode) {
        if (this->file_data != nullptr && this->file_size != 0) {
            const size_t chunk_size = 2048;
            size_t sent_bytes = 0;
            while (sent_bytes < this->file_size) {
                size_t remaining_bytes = this->file_size - sent_bytes;
                size_t sending_bytes = min(remaining_bytes, chunk_size);
                this->client_ptr->write(&this->file_data[sent_bytes], sending_bytes);
                sent_bytes += sending_bytes;
            }
        }
    } else {
        if (this->body != "") {
            this->client_ptr->print(this->body);
        }
    }

    this->client_ptr->print("\r\n\r\n");


    unsigned long start = millis();
    while (client_ptr->available() == 0) {
        if (millis() - start > this->timeout) {
            client_ptr->stop();
            result.network_error = true;
            result.error_message = "Connection timeout";
            return result;
        }
    }

    while (client_ptr->available()) {
        String line = client_ptr->readStringUntil('\n');
        this->response_ptr->add(line);
    }
    this->response_ptr->concatChunk();


    result.http_message = this->response_ptr->checkHttpMessage();
    result.http_version = this->response_ptr->checkHttpVersion();
    result.status_code = this->response_ptr->checkStatusCode();
    result.reason_phrase = this->response_ptr->checkReasonPhrase();
    result.headers = this->response_ptr->checkHeaders();
    result.message_body = this->response_ptr->checkMessageBody();
    result.ok = true;

    this->response_ptr->end();

    return result;
}

bool HttpRequest::end() {
    client_ptr->stop();
    this->path = "/";
    this->params = "?";
    this->header = "";
    this->body = "";
    this->request_line = "";
    this->file_data = nullptr;
    this->file_size = 0;
    this->file_mode = false;
    return true;
}