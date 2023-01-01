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

    HttpResponse response;

    std::unique_ptr<HttpClient> _client_ptr;
    HttpClient* client_ptr;

    std::unique_ptr<PostgRest> _postgrest_ptr;
    PostgRest* postgrest_ptr;

    std::unique_ptr<GoTrue> _gotrue_ptr;
    GoTrue* gotrue_ptr;


  public:
    Supabase();
    ~Supabase();

    // Auth(SupabaseAuthKeeper)
    GoTrue* auth() { return this->gotrue_ptr; }

    // Rest(SupabasePostgreHandler)
    PostgRest* rest() { return this->postgrest_ptr; }

    // Storage(SupabaseStorageHandler)
    // bool upload(char* bucket, char* file_path);
    // bool download(char* bucket, char* file_path);

    // Test
    HttpResponse CheckLatestHttpResponse() { return this->response; }
    const char* checkTable() { return this->table; }
    const char* checkColumn() { return this->column; }
    const char* checkJson() { return this->json; }
    const char* checkQuery() { return this->query; }
    const char* checkHeader() { return this->header; }
};

#endif