#ifndef HttpRequest_h
#define HttpRequest_h

#include "HttpResponse.h"
#include <WiFiClientSecure.h>
#include <memory>

struct Result {
    String http_message = "";
    String http_version = "";
    int status_code = 0;
    String reason_phrase = "";
    HttpHeader headers;
    String message_body = "";
    bool network_error = false;
    String error_message = "";
};

class HttpRequest {
  protected:
  private:
    String host = "";
    unsigned int port = 0;

    std::unique_ptr<WiFiClientSecure> _client_ptr =
        std::unique_ptr<WiFiClientSecure>(new WiFiClientSecure());
    WiFiClientSecure* client_ptr = _client_ptr.get();

    HttpResponse* response_ptr;

    String path = "/";
    String params = "?";
    String header = "";
    String body = "";
    String request_line = "";

    unsigned long timeout = 10000;


  public:
    HttpRequest(HttpResponse* response_ptr);
    ~HttpRequest();

    bool setCACert(const char* root_ca);
    bool setCertificate(const char* client_cert);
    bool setPrivateKey(const char* client_key);
    bool setInsecure();

    bool setHost(String host);
    bool setPort(unsigned int port);
    bool setPath(String path);

    bool addParameter(String key, String value);
    bool addHeader(String key, String value);
    bool setBody(String body);

    bool methodIsGet();
    bool methodIsPost();
    bool methodIsPut();
    bool methodIsPatch();
    bool methodIsDelete();
    bool methodIsHead();

    Result send();

    bool end();

    // Test
    String checkHost() const { return this->host; }
    unsigned int checkPort() const { return this->port; }
    String checkPath() const { return this->path; }
    String checkParams() const { return this->params; }
    String checkRequestLine() const { return this->request_line; }
    String checkHeader() const { return this->header; }
    String checkBody() const { return this->body; }
};

#endif
