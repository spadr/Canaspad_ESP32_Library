#include "PostgRest.h"


PostgRest::PostgRest(HttpClient* client_ptr, const char* path, const int port) {
    this->backend_path = path;
    this->backend_port = port;
    this->client_ptr = client_ptr;
}


PostgRest::~PostgRest() {}

PostgRest& PostgRest::begin(const char* host) {
    this->backend_host = host;
    return *this;
}

PostgRest& PostgRest::begin(const char* host, GoTrue* gotrue_ptr) {
    this->backend_host = host;
    this->gotrue_ptr = gotrue_ptr;
    this->use_token = true;

    String access_token = this->gotrue_ptr->useAccessToken();
    client_ptr->addHeader("Authorization", "Bearer " + access_token);
    if (access_token == "") {
        this->error = true;
        this->error_message = "PostgRest: Access token is empty";
        return *this;
    }

    return *this;
}

PostgRest& PostgRest::from(String table) {
    client_ptr->setPath(String(this->backend_path) + "/" + table);
    return *this;
}

PostgRest& PostgRest::select(String column) {
    column.replace(" ", "");
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

/*PostgRest& PostgRest::update(String json) {
    client_ptr->setBody(json);
    client_ptr->methodIsPatch();
    client_ptr->addHeader("Content-Length", String(json.length()));
    client_ptr->addHeader("Prefer", "return=representation");
    return *this;
}*/

/*PostgRest& PostgRest::delete_() {
    client_ptr->methodIsDelete();
    client_ptr->addHeader("Prefer", "return=representation");
    return *this;
}*/


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
    client_ptr->addHeader("Connection", "close");

    // TODO : check if the client is ready to execute
    this->result = client_ptr->send();
    client_ptr->end();

    if (this->result.network_error) {
        this->error = true;
        this->error_message =
            "PostgRest: Network error is occured " + String(this->result.error_message);
        return *this;
    }

    int status_code = this->result.status_code;
    if (status_code == 200 || status_code == 201) {
        this->error = false;
        this->error_message = "";
        return *this;
    }

    this->error = true;
    this->error_message = "PostgRest: HTTP Request failed " + String(status_code) + " " +
                          String(this->result.reason_phrase);
    return *this;
}