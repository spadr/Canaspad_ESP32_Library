#include "HttpRequest.h"


HttpRequest::HttpRequest(String host, unsigned int port) {
    this->host = host;
    this->port = port;
}

HttpRequest::~HttpRequest() {
    if (this->client_ptr) {
        this->client_ptr->stop();
    }
    if (this->_client_ptr) {
        _client_ptr.reset(nullptr);
    }
    if (this->_response_ptr) {
        _response_ptr.reset(nullptr);
    }
}

bool HttpRequest::setPath(String path) {
    this->path = path;
    return true;
}

bool HttpRequest::setInsecure() {
    this->client_ptr->setInsecure();
    return true;
}

bool HttpRequest::setCertificate(const char* cert) {
    this->client_ptr->setCertificate(cert);
    return true;
}

bool HttpRequest::setPrivateKey(const char* key) {
    this->client_ptr->setPrivateKey(key);
    return true;
}

bool HttpRequest::setCACert(const char* cert) {
    this->client_ptr->setCACert(cert);
    return true;
}

bool HttpRequest::begin() {
    const char* host = this->host.c_str();
    int port = this->port;
    if (!this->client_ptr->connect(host, port)) {
        log_e("Connection failed");
        log_e("host: %s", this->host.c_str());
        log_e("port: %d", this->port);
        return false;
    }
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

bool HttpRequest::_sendRequest(String request_line) {
    if (!this->client_ptr->connected()) {
        log_e("Connection lost");
        return false;
    }

    this->client_ptr->print(request_line);
    this->client_ptr->print(this->header);
    this->client_ptr->print("\r\n");
    if (this->body != "") {
        this->client_ptr->print(this->body);
    }
    this->client_ptr->print("\r\n\r\n");

    unsigned long timeout = millis();
    while (client_ptr->available() == 0) {
        if (millis() - timeout > 10000) {
            client_ptr->stop();
            log_e("HttpRequest: Connection timeout");
            return false;
        }
    }

    while (client_ptr->available()) {
        String line = client_ptr->readStringUntil('\n');
        this->response_ptr->add(line);
    }
    return true;
}

HttpResponse* HttpRequest::get() {
    if (!this->begin()) {
        return nullptr;
    }

    String request_target;
    if (this->params == "?") {
        request_target = this->path;
    } else {
        request_target = this->path + this->params;
    }

    String request_line = "GET " + request_target + " HTTP/1.1\r\n";

    if (!_sendRequest(request_line)) {
        return nullptr;
    }
    this->response_ptr->concatChunk();
    return this->response_ptr;
}

HttpResponse* HttpRequest::post() {
    if (!this->begin()) {
        return nullptr;
    }

    String request_target;
    if (this->params == "?") {
        request_target = this->path;
    } else {
        request_target = this->path + this->params;
    }

    String request_line = "POST " + request_target + " HTTP/1.1\r\n";

    if (!_sendRequest(request_line)) {
        return nullptr;
    }
    this->response_ptr->concatChunk();
    return this->response_ptr;
}

HttpResponse* HttpRequest::put() {
    if (!this->begin()) {
        return nullptr;
    }

    String request_target;
    if (this->params == "?") {
        request_target = this->path;
    } else {
        request_target = this->path + this->params;
    }

    String request_line = "PUT " + request_target + " HTTP/1.1\r\n";

    if (!_sendRequest(request_line)) {
        return nullptr;
    }
    this->response_ptr->concatChunk();
    return this->response_ptr;
}

HttpResponse* HttpRequest::patch() {
    if (!this->begin()) {
        return nullptr;
    }

    String request_target;
    if (this->params == "?") {
        request_target = this->path;
    } else {
        request_target = this->path + this->params;
    }

    String request_line = "PATCH " + request_target + " HTTP/1.1\r\n";

    if (!_sendRequest(request_line)) {
        return nullptr;
    }
    this->response_ptr->concatChunk();
    return this->response_ptr;
}

HttpResponse* HttpRequest::delete_() {
    if (!this->begin()) {
        return nullptr;
    }

    String request_target;
    if (this->params == "?") {
        request_target = this->path;
    } else {
        request_target = this->path + this->params;
    }

    String request_line = "DELETE " + request_target + " HTTP/1.1\r\n";

    if (!_sendRequest(request_line)) {
        return nullptr;
    }
    this->response_ptr->concatChunk();
    return this->response_ptr;
}