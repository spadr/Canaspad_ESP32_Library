#include "PostgRest.h"


PostgRest::PostgRest(HttpClient** client_pp, const char* path, const int port)
    : backend_path(path), backend_port(port), client_pp(client_pp) {}


PostgRest::~PostgRest() {}

PostgRest& PostgRest::begin(const char* host) {
    this->backend_host = host;
    return *this;
}

PostgRest& PostgRest::begin(const char* host, GoTrue** gotrue_pp) {
    this->backend_host = host;
    this->gotrue_pp = gotrue_pp;
    this->use_token = true;

    GoTrue& auth_client = **this->gotrue_pp;
    HttpClient& client = **client_pp;
    String access_token = auth_client.useAccessToken();
    if (access_token == "") {
        this->error = true;
        this->error_message = "PostgRest: Access token is empty";
        return *this;
    }

    client.addHeader("Authorization", "Bearer " + access_token);

    return *this;
}

PostgRest& PostgRest::from(String table) {
    HttpClient& client = **client_pp;
    client.setPath(String(this->backend_path) + "/" + table);
    return *this;
}

PostgRest& PostgRest::select(String column) {
    column.replace(" ", "");
    HttpClient& client = **client_pp;
    client.addParameter("select", column);
    client.methodIsGet();
    return *this;
}

PostgRest& PostgRest::insert(String json) {
    HttpClient& client = **client_pp;
    client.setBody(json);
    client.methodIsPost();
    client.addHeader("Content-Length", String(json.length()));
    client.addHeader("Prefer", "return=representation");
    return *this;
}

PostgRest& PostgRest::upsert(String json) {
    HttpClient& client = **client_pp;
    client.setBody(json);
    client.methodIsPost();
    client.addHeader("Content-Length", String(json.length()));
    client.addHeader("Prefer", "resolution=merge-duplicates");
    client.addHeader("Prefer", "return=representation");
    return *this;
}

PostgRest& PostgRest::update(String json) {
    HttpClient& client = **client_pp;
    client.setBody(json);
    client.methodIsPatch();
    client.addHeader("Content-Length", String(json.length()));
    client.addHeader("Prefer", "return=representation");
    return *this;
}

/*PostgRest& PostgRest::delete_() {
    HttpClient& client = **client_pp;
    client.methodIsDelete();
    client.addHeader("Prefer", "return=representation");
    return *this;
}*/


PostgRest& PostgRest::eq(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "eq." + value);
    return *this;
}

PostgRest& PostgRest::gt(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "gt." + value);
    return *this;
}

PostgRest& PostgRest::gte(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "gte." + value);
    return *this;
}

PostgRest& PostgRest::lt(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "lt." + value);
    return *this;
}

PostgRest& PostgRest::lte(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "lte." + value);
    return *this;
}

PostgRest& PostgRest::neq(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "neq." + value);
    return *this;
}

PostgRest& PostgRest::like(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "like." + value);
    return *this;
}

PostgRest& PostgRest::ilike(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "ilike." + value);
    return *this;
}

PostgRest& PostgRest::match(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "match." + value);
    return *this;
}

PostgRest& PostgRest::imatch(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "match." + value);
    return *this;
}

PostgRest& PostgRest::is(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "is." + value);
    return *this;
}

PostgRest& PostgRest::in(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "in." + value);
    return *this;
}

PostgRest& PostgRest::fts(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "fts." + value);
    return *this;
}

PostgRest& PostgRest::plfts(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "plfts." + value);
    return *this;
}

PostgRest& PostgRest::phfts(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "phfts." + value);
    return *this;
}

PostgRest& PostgRest::wfts(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "wfts." + value);
    return *this;
}

PostgRest& PostgRest::cs(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "cs." + value);
    return *this;
}

PostgRest& PostgRest::cd(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "cd." + value);
    return *this;
}

PostgRest& PostgRest::ov(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "ov." + value);
    return *this;
}

PostgRest& PostgRest::sl(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "sl." + value);
    return *this;
}

PostgRest& PostgRest::sr(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "sr." + value);
    return *this;
}

PostgRest& PostgRest::nxr(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "nxr." + value);
    return *this;
}

PostgRest& PostgRest::nxl(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "nxl." + value);
    return *this;
}

PostgRest& PostgRest::adj(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "adj." + value);
    return *this;
}

PostgRest& PostgRest::not_(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "not." + value);
    return *this;
}

PostgRest& PostgRest::or_(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "or." + value);
    return *this;
}


PostgRest& PostgRest::and_(String column, String value) {
    HttpClient& client = **client_pp;
    client.addParameter(column, "and." + value);
    return *this;
}

PostgRest& PostgRest::order(String column, bool ascend) {
    HttpClient& client = **client_pp;
    if (ascend) {
        client.addParameter("order", column + ".asc");
    } else {
        client.addParameter("order", column + ".desc");
    }
    return *this;
}

PostgRest& PostgRest::limit(int limit, int offset) {
    HttpClient& client = **client_pp;
    if (limit != -1) {
        client.addParameter("limit", String(limit));
    }
    if (offset != -1) {
        client.addParameter("offset", String(offset));
    }
    return *this;
}

PostgRest& PostgRest::execute() {
    HttpClient& client = **client_pp;
    client.setPort(this->backend_port);
    client.addHeader("Host", String(this->backend_host));
    client.addHeader("User-Agent", "Canaspad_ESP32_Library/0.3");
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Connection", "close");

    // TODO : check if the client is ready to execute
    this->result = client.send();
    client.end();

    if (this->result.ok) {
        return *this;
    } else {
        if (this->result.network_error) {
            this->error = true;
            this->error_message =
                "PostgRest: Network error is occured " + String(this->result.error_message);
            return *this;
        }

        int status_code = this->result.status_code;
        if (status_code >= 200 && status_code < 300) {
            this->error = false;
            this->error_message = "";
            return *this;
        }

        this->error = true;
        this->error_message = "PostgRest: HTTP Request failed " + String(status_code) + " " +
                              String(this->result.reason_phrase);

        return *this;
    }
}