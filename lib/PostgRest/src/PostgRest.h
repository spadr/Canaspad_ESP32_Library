#ifndef PostgRest_h
#define PostgRest_h

#include <Arduino.h>
#include <GoTrue.h>
#include <HttpClient.h>

class PostgRest {
  protected:
  private:
    HttpClient* client_ptr;

    const char* backend_host;
    int backend_port;
    const char* backend_path;

    bool use_token = false;
    GoTrue* gotrue_ptr;

    bool error = false;
    String error_message = "";
    Result result;

  public:
    PostgRest(HttpClient* client_ptr, const char* path, const int port);
    ~PostgRest();

    PostgRest& begin(const char* host);
    PostgRest& begin(const char* host, GoTrue* gotrue_ptr);

    PostgRest& from(String table);
    PostgRest& select(String column);
    PostgRest& insert(String json);
    PostgRest& upsert(String json);
    // PostgRest& update(String json);
    // PostgRest& delete_();


    PostgRest& eq(String column, String value);
    PostgRest& gt(String column, String value);
    PostgRest& gte(String column, String value);
    PostgRest& lt(String column, String value);
    PostgRest& lte(String column, String value);
    PostgRest& neq(String column, String value);
    PostgRest& like(String column, String value);
    PostgRest& ilike(String column, String value);
    PostgRest& match(String column, String value);
    PostgRest& imatch(String column, String value);
    PostgRest& in(String column, String value);
    PostgRest& is(String column, String value);
    PostgRest& fts(String column, String value);
    PostgRest& plfts(String column, String value);
    PostgRest& phfts(String column, String value);
    PostgRest& wfts(String column, String value);
    PostgRest& cs(String column, String value);
    PostgRest& cd(String column, String value);
    PostgRest& ov(String column, String value);
    PostgRest& sl(String column, String value);
    PostgRest& sr(String column, String value);
    PostgRest& nxr(String column, String value);
    PostgRest& nxl(String column, String value);
    PostgRest& adj(String column, String value);
    PostgRest& not_(String column, String value);
    PostgRest& or_(String column, String value);
    PostgRest& and_(String column, String value);

    PostgRest& order(String column, bool ascend = true);
    PostgRest& limit(int limit = -1, int offset = -1);

    PostgRest& execute();

    // Result
    bool checkError() { return this->error; }
    String checkErrorMessage() { return this->error_message; }
    String checkResult() { return this->result.message_body; }
};

#endif