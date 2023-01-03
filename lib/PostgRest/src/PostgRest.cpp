#include "PostgRest.h"


PostgRest::PostgRest(const char* path, const int port) {
    this->backend_path = path;
    this->backend_port = port;
}


PostgRest::~PostgRest() {
    if (this->_client_ptr) {
        _client_ptr.reset(nullptr);
    }
}

PostgRest& PostgRest::begin(const char* host, const char* key) {
    this->backend_host = host;
    this->backend_key = key;
    this->_client_ptr =
        std::unique_ptr<HttpClient>(new HttpClient(this->backend_host, this->backend_port));
    this->client_ptr = _client_ptr.get();
    this->client_ptr->setInsecure();
    return *this;
}

PostgRest& PostgRest::begin(const char* host, const char* key, GoTrue* gotrue_ptr) {
    this->backend_host = host;
    this->backend_key = key;
    this->gotrue_ptr = gotrue_ptr;
    this->use_token = true;
    this->_client_ptr =
        std::unique_ptr<HttpClient>(new HttpClient(this->backend_host, this->backend_port));
    this->client_ptr = _client_ptr.get();
    this->client_ptr->setInsecure();
    return *this;
}

PostgRest& PostgRest::from(String table) {
    client_ptr->setPath(String(this->backend_path) + "/" + table);
    return *this;
}

PostgRest& PostgRest::select(String column) {
    client_ptr->addParameter("select", column);
    client_ptr->methodIsGet();
    return *this;
}

PostgRest& PostgRest::insert(String json) {
    client_ptr->setBody(json);
    client_ptr->methodIsPost();
    client_ptr->addHeader("Content-Length", String(json.length()));
    client_ptr->addHeader("Prefer", "return=representation");
    return *this;
}

PostgRest& PostgRest::upsert(String json) {
    client_ptr->setBody(json);
    client_ptr->methodIsPost();
    client_ptr->addHeader("Content-Length", String(json.length()));
    client_ptr->addHeader("Prefer", "resolution=merge-duplicates");
    client_ptr->addHeader("Prefer", "return=representation");
    return *this;
}

PostgRest& PostgRest::update(String json) {
    client_ptr->setBody(json);
    client_ptr->methodIsPatch();
    client_ptr->addHeader("Content-Length", String(json.length()));
    client_ptr->addHeader("Prefer", "return=representation");
    return *this;
}

PostgRest& PostgRest::delete_() {
    client_ptr->methodIsDelete();
    client_ptr->addHeader("Prefer", "return=representation");
    return *this;
}


PostgRest& PostgRest::eq(String column, String value) {
    client_ptr->addParameter(column, "eq." + value);
    return *this;
}

PostgRest& PostgRest::gt(String column, String value) {
    client_ptr->addParameter(column, "gt." + value);
    return *this;
}

PostgRest& PostgRest::gte(String column, String value) {
    client_ptr->addParameter(column, "gte." + value);
    return *this;
}

PostgRest& PostgRest::lt(String column, String value) {
    client_ptr->addParameter(column, "lt." + value);
    return *this;
}

PostgRest& PostgRest::lte(String column, String value) {
    client_ptr->addParameter(column, "lte." + value);
    return *this;
}

PostgRest& PostgRest::neq(String column, String value) {
    client_ptr->addParameter(column, "neq." + value);
    return *this;
}

PostgRest& PostgRest::like(String column, String value) {
    client_ptr->addParameter(column, "like." + value);
    return *this;
}

PostgRest& PostgRest::ilike(String column, String value) {
    client_ptr->addParameter(column, "ilike." + value);
    return *this;
}

PostgRest& PostgRest::match(String column, String value) {
    client_ptr->addParameter(column, "match." + value);
    return *this;
}

PostgRest& PostgRest::imatch(String column, String value) {
    client_ptr->addParameter(column, "match." + value);
    return *this;
}

PostgRest& PostgRest::is(String column, String value) {
    client_ptr->addParameter(column, "is." + value);
    return *this;
}

PostgRest& PostgRest::in(String column, String value) {
    client_ptr->addParameter(column, "in." + value);
    return *this;
}

PostgRest& PostgRest::fts(String column, String value) {
    client_ptr->addParameter(column, "fts." + value);
    return *this;
}

PostgRest& PostgRest::plfts(String column, String value) {
    client_ptr->addParameter(column, "plfts." + value);
    return *this;
}

PostgRest& PostgRest::phfts(String column, String value) {
    client_ptr->addParameter(column, "phfts." + value);
    return *this;
}

PostgRest& PostgRest::wfts(String column, String value) {
    client_ptr->addParameter(column, "wfts." + value);
    return *this;
}

PostgRest& PostgRest::cs(String column, String value) {
    client_ptr->addParameter(column, "cs." + value);
    return *this;
}

PostgRest& PostgRest::cd(String column, String value) {
    client_ptr->addParameter(column, "cd." + value);
    return *this;
}

PostgRest& PostgRest::ov(String column, String value) {
    client_ptr->addParameter(column, "ov." + value);
    return *this;
}

PostgRest& PostgRest::sl(String column, String value) {
    client_ptr->addParameter(column, "sl." + value);
    return *this;
}

PostgRest& PostgRest::sr(String column, String value) {
    client_ptr->addParameter(column, "sr." + value);
    return *this;
}

PostgRest& PostgRest::nxr(String column, String value) {
    client_ptr->addParameter(column, "nxr." + value);
    return *this;
}

PostgRest& PostgRest::nxl(String column, String value) {
    client_ptr->addParameter(column, "nxl." + value);
    return *this;
}

PostgRest& PostgRest::adj(String column, String value) {
    client_ptr->addParameter(column, "adj." + value);
    return *this;
}

PostgRest& PostgRest::not_(String column, String value) {
    client_ptr->addParameter(column, "not." + value);
    return *this;
}

PostgRest& PostgRest::or_(String column, String value) {
    client_ptr->addParameter(column, "or." + value);
    return *this;
}


PostgRest& PostgRest::and_(String column, String value) {
    client_ptr->addParameter(column, "and." + value);
    return *this;
}

PostgRest& PostgRest::order(String column, bool ascend) {
    if (ascend) {
        client_ptr->addParameter("order", column + ".asc");
    } else {
        client_ptr->addParameter("order", column + ".desc");
    }
    return *this;
}

PostgRest& PostgRest::limit(int limit, int offset) {
    if (limit != -1) {
        client_ptr->addParameter("limit", String(limit));
    }
    if (offset != -1) {
        client_ptr->addParameter("offset", String(offset));
    }
    return *this;
}

PostgRest& PostgRest::execute() {
    client_ptr->addHeader("Host", String(this->backend_host));
    client_ptr->addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client_ptr->addHeader("Content-Type", "application/json");
    client_ptr->addHeader("apikey", this->backend_key);

    if (this->use_token) {
        String access_token = this->gotrue_ptr->useAccessToken();
        client_ptr->addHeader("Authorization", "Bearer " + access_token);
    }

    client_ptr->addHeader("Connection", "close");

    // TODO : check if the client is ready to execute
    HttpResponse* res_ptr = client_ptr->send();

    client_ptr->end();

    if (res_ptr->checkNetworkError()) {
        this->error = true;
        this->error_message =
            "PostgRest: Network error is occured " + String(res_ptr->checkErrorMessage());
        return *this;
    }

    int status_code = res_ptr->checkStatusCode();

    if (int(status_code / 100) != 2) {
        this->error = true;
        this->error_message = "PostgRest: HTTP Request failed " + String(status_code) + " " +
                              String(res_ptr->checkReasonPhrase());
    }
    this->error = false;
    this->error_message = "";
    this->response = res_ptr;

    return *this;
}