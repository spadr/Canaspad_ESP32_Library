#ifndef HttpClient_h
#define HttpClient_h

#include <WiFi.h>

struct Response {
    int status;
    String body;
};

typedef String url_t;

class HttpClient {
  protected:
    WiFiClient client;

  private:
  public:
    HttpClient();
    ~HttpClient();

    Response get(const char* url, const char* header);
    Response post(const char* url, const char* header, const char* body);
    Response put(const char* url, const char* header, const char* body);
    Response patch(const char* url, const char* header, const char* body);
    Response delete_(const char* url, const char* header);
};

#endif
