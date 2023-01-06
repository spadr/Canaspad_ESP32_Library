#ifndef Supabase_h
#define Supabase_h

#include <Arduino.h>
#include <GoTrue.h>
#include <HttpClient.h>
#include <PostgRest.h>
#include <memory>


class Supabase {
  protected:
  private:
    const char* backend_auth_path = "/auth/v1";
    const char* backend_rest_path = "/rest/v1";
    const char* backend_storage_path = "/storage/v1";

    const int backend_port = 443;


    char* table;
    char* column;
    char* json;
    char* query;
    char* header;

    std::unique_ptr<HttpClient> _http_client_ptr;
    HttpClient* http_client_ptr;

    std::unique_ptr<HttpResponse> _http_response_ptr;
    HttpResponse* http_response_ptr;

    std::unique_ptr<PostgRest> _postgrest_ptr;
    PostgRest* postgrest_ptr;

    std::unique_ptr<GoTrue> _gotrue_ptr;
    GoTrue* gotrue_ptr;


  public:
    Supabase();
    ~Supabase();

    void setCACert(const char* root_ca) { this->http_client_ptr->setCACert(root_ca); }

    void setCertificate(const char* client_cert) {
        this->http_client_ptr->setCertificate(client_cert);
    }
    void setPrivateKey(const char* client_key) { this->http_client_ptr->setPrivateKey(client_key); }
    void setInsecure() { this->http_client_ptr->setInsecure(); }

    // Auth(SupabaseAuthKeeper)
    GoTrue* auth() { return this->gotrue_ptr; }

    // Rest(SupabasePostgreHandler)
    PostgRest* rest() { return this->postgrest_ptr; }

    // Storage(SupabaseStorageHandler)
    // bool upload(char* bucket, char* file_path);
    // bool download(char* bucket, char* file_path);

    // Test
    // HttpResponse* CheckLatestHttpResponse() { return this->http_response_ptr; }
};

#endif