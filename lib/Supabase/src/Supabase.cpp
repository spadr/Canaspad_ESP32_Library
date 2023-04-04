#include "Supabase.h"

Supabase::Supabase() {
    this->_http_response_ptr = std::unique_ptr<HttpResponse>(new HttpResponse());
    this->http_response_ptr = _http_response_ptr.get();

    this->_http_client_ptr = std::unique_ptr<HttpClient>(new HttpClient(this->http_response_ptr));
    this->http_client_ptr = _http_client_ptr.get();

    this->_gotrue_ptr = std::unique_ptr<GoTrue>(
        new GoTrue(&this->http_client_ptr, this->backend_auth_path, this->backend_port));
    this->gotrue_ptr = _gotrue_ptr.get();

    this->_postgrest_ptr = std::unique_ptr<PostgRest>(
        new PostgRest(&this->http_client_ptr, this->backend_rest_path, this->backend_port));
    this->postgrest_ptr = _postgrest_ptr.get();

    this->_storage_ptr = std::unique_ptr<Storage>(
        new Storage(&this->http_client_ptr, this->backend_storage_path, this->backend_port));
    this->storage_ptr = _storage_ptr.get();
}

Supabase::~Supabase() {
    if (this->http_client_ptr) {
        this->http_client_ptr->end();
    }
    if (this->_http_client_ptr) {
        _http_client_ptr.reset(nullptr);
    }
    if (this->_postgrest_ptr) {
        _postgrest_ptr.reset(nullptr);
    }
    if (this->_storage_ptr) {
        _storage_ptr.reset(nullptr);
    }
    if (this->_gotrue_ptr) {
        _gotrue_ptr.reset(nullptr);
    }
    if (this->_http_client_ptr) {
        _http_client_ptr.reset(nullptr);
    }
    if (this->_http_response_ptr) {
        _http_response_ptr.reset(nullptr);
    }
}