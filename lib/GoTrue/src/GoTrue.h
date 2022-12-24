#ifndef GoTrue_h
#define GoTrue_h

#include <Arduino.h>
#include <HttpClient.h>

class GoTrue {
  protected:
  private:
    const char* gotrue_host;
    const char* gotrue_key;

    const char* backend_path = "";
    const char* endpoint = "";
    int backend_port;

    bool signed_in = false;

    const char* email;    // TODO encryption
    const char* password; // TODO encryption
    String access_token;  // TODO encryption
    String refresh_token; // TODO encryption

    HttpResponse response;
    bool error = false;
    String error_message = "";

    bool setAccessToken();
    bool refreshAccessToken();


  public:
    GoTrue(const char* path, const int port);
    ~GoTrue();

    GoTrue& signIn(const char* host, const char* key = NULL, const char* username = NULL,
                   const char* password = NULL, const char* auth_endpoint = NULL);
    GoTrue& keepAuth();

    // Test
    bool checkAuthStatus() { return this->signed_in; }
    String checkAccessToken() { return this->access_token; }
    String checkRefreshToken() { return this->refresh_token; }

    // Result
    bool checkError() { return this->error; }
    String checkErrorMessage() { return this->error_message; }
    String checkResult() { return this->response.checkMessageBody(); }
};

#endif