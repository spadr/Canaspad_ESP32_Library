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

HttpResponse* HttpRequest::send() {

    if (!this->client_ptr->connect(this->host.c_str(), this->port)) {
        log_e("Connection failed");
        return nullptr;
    }

    if (!this->client_ptr->connected()) {
        log_e("Connection lost");
        return nullptr;
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

    if (this->body != "") {
        this->client_ptr->print(this->body);
    }
    this->client_ptr->print("\r\n\r\n");


    unsigned long timeout = millis();
    while (client_ptr->available() == 0) {
        if (millis() - timeout > 1000) {
            client_ptr->stop();
            log_e("HttpRequest: Connection timeout");
            return nullptr;
        }
    }

    while (client_ptr->available()) {
        String line = client_ptr->readStringUntil('\n');
        this->response_ptr->add(line);
    }
    this->response_ptr->concatChunk();
    return this->response_ptr;
}

bool HttpRequest::end() {
    client_ptr->stop();
    return true;
}