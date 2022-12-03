#include "HttpClient.h"

HttpClient::HttpClient() {}
HttpClient::~HttpClient() {}

Response HttpClient::get(const char* url, const char* header) {
    Response response{};
    if (client.connect(url, 80)) {
        client.println("GET " + String(url) + " HTTP/1.1");
        client.println("Host: " + String(url));
        client.println("User-Agent: Arduino/1.0");
        client.println("Connection: close");
        client.println("Accept: application/json");
        client.println("Authorization: Bearer " + String(header));
        client.println();
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                break;
            }
        }
        String body = client.readStringUntil('\n');
        response.status = 200;
        response.body = body;
    } else {
        response.status = 500;
        response.body = "Connection failed";
    }
    return response;
}

Response HttpClient::post(const char* url, const char* header, const char* body) {
    Response response{};
    if (client.connect(url, 80)) {
        client.println("POST " + String(url) + " HTTP/1.1");
        client.println("Host: " + String(url));
        client.println("User-Agent: Arduino/1.0");
        client.println("Connection: close");
        client.println("Accept: application/json");
        client.println("Authorization: Bearer " + String(header));
        client.println("Content-Type: application/json");
        client.println("Content-Length: " + String(strlen(body)));
        client.println();
        client.println(body);
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                break;
            }
        }
        String body = client.readStringUntil('\n');
        response.status = 200;
        response.body = body;
    } else {
        response.status = 500;
        response.body = "Connection failed";
    }
    return response;
}

Response HttpClient::put(const char* url, const char* header, const char* body) {
    Response response{};
    if (client.connect(url, 80)) {
        client.println("PUT " + String(url) + " HTTP/1.1");
        client.println("Host: " + String(url));
        client.println("User-Agent: Arduino/1.0");
        client.println("Connection: close");
        client.println("Accept: application/json");
        client.println("Authorization: Bearer " + String(header));
        client.println("Content-Type: application/json");
        client.println("Content-Length: " + String(strlen(body)));
        client.println();
        client.println(body);
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                break;
            }
        }
        String body = client.readStringUntil('\n');
        response.status = 200;
        response.body = body;
    } else {
        response.status = 500;
        response.body = "Connection failed";
    }
    return response;
}

Response HttpClient::patch(const char* url, const char* header, const char* body) {
    Response response{};
    if (client.connect(url, 80)) {
        client.println("PATCH " + String(url) + " HTTP/1.1");
        client.println("Host: " + String(url));
        client.println("User-Agent: Arduino/1.0");
        client.println("Connection: close");
        client.println("Accept: application/json");
        client.println("Authorization: Bearer " + String(header));
        client.println("Content-Type: application/json");
        client.println("Content-Length: " + String(strlen(body)));
        client.println();
        client.println(body);
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                break;
            }
        }
        String body = client.readStringUntil('\n');
        response.status = 200;
        response.body = body;
    } else {
        response.status = 500;
        response.body = "Connection failed";
    }
    return response;
}

Response HttpClient::delete_(const char* url, const char* header) {
    Response response{};
    if (client.connect(url, 80)) {
        client.println("DELETE " + String(url) + " HTTP/1.1");
        client.println("Host: " + String(url));
        client.println("User-Agent: Arduino/1.0");
        client.println("Connection: close");
        client.println("Accept: application/json");
        client.println("Authorization: Bearer " + String(header));
        client.println();
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                break;
            }
        }
        String body = client.readStringUntil('\n');
        response.status = 200;
        response.body = body;
    } else {
        response.status = 500;
        response.body = "Connection failed";
    }
    return response;
}
