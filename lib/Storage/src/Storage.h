#ifndef Storage_h
#define Storage_h

#include <Arduino.h>
#include <GoTrue.h>
#include <HttpClient.h>

enum class ContentType {
    JSON,
    CSV,
    TEXT,
    XML,
    PNG,
    JPEG,
    GIF,
    SVG,
    PDF,
    ZIP,
    MP3,
    MP4,
    OGG,
    WAV,
    WEBM,
    OTHER
};

class Storage {
  protected:
  private:
    HttpClient** client_pp;

    const char* backend_host;
    int backend_port;
    const char* backend_path;

    bool use_token = false;
    GoTrue** gotrue_pp;

    bool error = false;
    String error_message = "";
    Result result;

    String storage_path;
    String file_name;

  public:
    Storage(HttpClient** client_pp, const char* path, const int port);
    ~Storage();

    Storage& begin(const char* host);
    Storage& begin(const char* host, GoTrue** gotrue_pp);

    Storage& from(String path);
    Storage& to(String path) { return from(path); }
    Storage& in(String path) { return from(path); }

    // Storage& download(String file_name, uint8_t* pick_value, size_t* size);
    Storage& upload(String file_name, ContentType content_type, uint8_t* payload, size_t size);
    // Storage& fix(String file_name, ContentType content_type, uint8_t* payload, size_t size);
    // Storage& remove(String file_name);

    Storage& execute();

    bool checkError() { return this->error; }
    String checkErrorMessage() { return this->error_message; }
    String checkResult() { return this->result.message_body; }
    String checkPath() {
        HttpClient& http_client = **client_pp;
        return http_client.checkHost() + String(this->backend_path);
    }
};

#endif
