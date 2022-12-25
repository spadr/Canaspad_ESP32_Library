#ifndef HttpResponse_h
#define HttpResponse_h

#include "HttpHeader.h"
#include <Arduino.h>
#include <ArduinoJson.h>

class HttpResponse {
  protected:
  private:
    String http_message;
    String http_version;
    int status_code;
    String reason_phrase;

    HttpHeader headers;
    String message_body;

    bool status_line = true;
    bool header_field = true;

  public:
    HttpResponse();

    ~HttpResponse();

    void add(String line);
    void concatChunk();

    String checkHttpMessage() const { return this->http_message; }
    String checkHttpVersion() const { return this->http_version; }
    int checkStatusCode() const { return this->status_code; }
    String checkReasonPhrase() const { return this->reason_phrase; }
    HttpHeader checkHeaders() const { return this->headers; }
    String checkMessageBody() const { return this->message_body; }
};

#endif