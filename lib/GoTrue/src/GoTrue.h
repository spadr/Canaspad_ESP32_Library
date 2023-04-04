#ifndef GoTrue_h
#define GoTrue_h

#include <Arduino.h>
#include <HttpClient.h>

class GoTrue {
  protected:
  private:
    HttpClient** client_pp;

    const char* gotrue_host;

    const char* backend_path = "";
    const char* endpoint = "";
    int backend_port;

    bool is_signed_in = false;
    unsigned long signed_in_at = 0;
    unsigned long expires_in = 0;
    unsigned long expiration_offset = 5000;

    const char* email;    // TODO encryption
    const char* password; // TODO encryption
    String access_token;  // TODO encryption
    String refresh_token; // TODO encryption

    bool error = false;
    String error_message = "";
    Result result;

    bool setAccessToken();
    bool refreshAccessToken();


  public:
    GoTrue(HttpClient** client_pp, const char* path, const int port);
    ~GoTrue();

    GoTrue& signIn(const char* host, const char* username = NULL, const char* password = NULL,
                   const char* auth_endpoint = NULL);
    String useAccessToken();

    // Test
    bool checkAuthStatus() { return this->is_signed_in; }
    String checkAccessToken() { return this->access_token; }
    String checkRefreshToken() { return this->refresh_token; }

    // Result
    bool checkError() { return this->error; }
    String checkErrorMessage() { return this->error_message; }
    // String checkResult() { return this->response.checkMessageBody(); }
};

#endif