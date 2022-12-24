#ifndef HttpRequest_h
#define HttpRequest_h

#include "HttpResponse.h"
#include <WiFiClientSecure.h>

class HttpRequest {
  protected:
  private:
    String host;
    unsigned int port;

    std::unique_ptr<WiFiClientSecure> _client_ptr =
        std::unique_ptr<WiFiClientSecure>(new WiFiClientSecure());
    std::unique_ptr<HttpResponse> _response_ptr = std::unique_ptr<HttpResponse>(new HttpResponse());

    WiFiClientSecure* client_ptr = _client_ptr.get();
    HttpResponse* response_ptr = _response_ptr.get();

    String path = "/";
    String params = "?";
    String header = "";
    String body = "";
    String request_line = "";


  public:
    HttpRequest(String host, unsigned int port);
    ~HttpRequest();

    bool setInsecure();
    bool setPath(String path);
    bool setCertificate(const char* cert);
    bool setPrivateKey(const char* key);
    bool setCACert(const char* cert);

    bool addParameter(String key, String value);
    bool addHeader(String key, String value);
    bool setBody(String body);

    bool methodIsGet();
    bool methodIsPost();
    bool methodIsPut();
    bool methodIsPatch();
    bool methodIsDelete();
    bool methodIsHead();

    HttpResponse* send();

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