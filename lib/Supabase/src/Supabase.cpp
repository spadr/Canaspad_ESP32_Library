#include "Supabase.h"

Supabase::Supabase() {
    this->_postgrest_ptr =
        std::unique_ptr<PostgRest>(new PostgRest(this->backend_rest_path, this->backend_port));
    this->postgrest_ptr = _postgrest_ptr.get();
    this->_gotrue_ptr =
        std::unique_ptr<GoTrue>(new GoTrue(this->backend_auth_path, this->backend_port));
    this->gotrue_ptr = _gotrue_ptr.get();
}

Supabase::~Supabase() {}
