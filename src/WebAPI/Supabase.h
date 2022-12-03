#ifndef Supabase_h
#define Supabase_h

#include "Http/HttpClient.h"
#include <Arduino.h>

// class SupabaseAuthKeeper {};

// class SupabasePostgreHandler {};

// class SupabaseStorageHandler {};


class Supabase {
  protected:
  private:
    const char* supabase_host;
    const char* supabase_key;

    const char* backend_auth_path = "/auth/v1";
    const char* backend_rest_url = "/rest/v1";
    const char* backend_storage_url = "/storage/v1";

    bool useAccessToken = false;

    const char* email;         // TODO encryption
    const char* password;      // TODO encryption
    const char* access_token;  // TODO encryption
    const char* refresh_token; // TODO encryption

    char* table;
    char* column;
    char* json;
    char* query;
    char* header;

    HttpResponse response;


  public:
    Supabase();
    ~Supabase();

    // Auth(SupabaseAuthKeeper)
    bool begin(const char* url, const char* key = NULL, const char* username = NULL,
               const char* password = NULL, const char* auth_endpoint = NULL);
    bool updateAccessToken();
    bool setAccessToken();
    bool refreshAccessToken();
    bool deleteAccessToken();

    // Postgres(SupabasePostgresHandler)
    bool from(char* table);
    bool select(char* column);
    bool insert(char* json);
    bool filter(char* query);
    HttpResponse execute(bool auth = true);
    // bool update(char* table, char* json);
    // bool delete(char* table);

    // Storage(SupabaseStorageHandler)
    // bool upload(char* bucket, char* file_path);
    // bool download(char* bucket, char* file_path);

    // Test
    HttpResponse CheckLatestHttpResponse() { return this->response; }
    const char* checkApiUrl() { return this->supabase_host; }
    const char* checkApiKey() { return this->supabase_key; }
    // const char* checkAuthUrl() { return this->full_auth_url_with_table_name; }
    // const char* checkRestUrl() { return this->full_rest_url; }
    // const char* checkStorageUrl() { return this->full_storage_url; }
    const char* checkEmail() { return this->email; }
    const char* checkPassword() { return this->password; }
    const char* checkAccessToken() { return this->access_token; }
    const char* checkRefreshToken() { return this->refresh_token; }
    const char* checkTable() { return this->table; }
    const char* checkColumn() { return this->column; }
    const char* checkJson() { return this->json; }
    const char* checkQuery() { return this->query; }
    const char* checkHeader() { return this->header; }
};

#endif