#include "HttpHeader.h"

HttpHeader::HttpHeader() {}
HttpHeader::~HttpHeader() {}

void HttpHeader::add(String name, String value) {
    if (name.equalsIgnoreCase("Date")) {
        this->Date += value;
    } else if (name.equalsIgnoreCase("Content-Type")) {
        this->Content_Type += value;
    } else if (name.equalsIgnoreCase("Transfer-Encoding")) {
        this->Transfer_Encoding += value;
    } else if (name.equalsIgnoreCase("Connection")) {
        this->Connection += value;
    } else if (name.equalsIgnoreCase("CF-Ray")) {
        this->CF_Ray += value;
    } else if (name.equalsIgnoreCase("Access-Control-Allow-Origin")) {
        this->Access_Control_Allow_Origin += value;
    } else if (name.equalsIgnoreCase("Set-Cookie")) {
        this->Set_Cookie += value;
    } else if (name.equalsIgnoreCase("Vary")) {
        this->Vary += value;
    } else if (name.equalsIgnoreCase("Via")) {
        this->Via += value;
    } else if (name.equalsIgnoreCase("CF-Cache-Status")) {
        this->CF_Cache_Status += value;
    } else if (name.equalsIgnoreCase("X-Kong-Proxy-Latency")) {
        this->X_Kong_Proxy_Latency += value;
    } else if (name.equalsIgnoreCase("X-Kong-Upstream-Latency")) {
        this->X_Kong_Upstream_Latency += value;
    } else if (name.equalsIgnoreCase("Server")) {
        this->Server += value;
    }
}