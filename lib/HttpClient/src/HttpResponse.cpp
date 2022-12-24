#include "HttpResponse.h"

HttpResponse::HttpResponse() {}
HttpResponse::~HttpResponse() {}


void HttpResponse::add(String line) {
    this->http_message += line;

    if (status_line) {
        status_line = false;
        int status_code_pos = line.indexOf(' ');
        int reason_phrase_pos = line.indexOf(' ', status_code_pos + 1);
        if (status_code_pos == -1 || reason_phrase_pos == -1) {
            this->status_code = 0;
            this->reason_phrase = "Unknown";
            return;
        }
        this->http_version = line.substring(0, status_code_pos);
        this->status_code = line.substring(status_code_pos + 1, reason_phrase_pos).toInt();
        this->reason_phrase = line.substring(reason_phrase_pos + 1);
        return;
    }

    if (header_field) {
        if (line == "\r") {
            // end of header
            header_field = false;
            return;
        }

        int colon_pos = line.indexOf(':');
        if (colon_pos == -1) {
            // invalid header
            return;
        }

        String header_name = line.substring(0, colon_pos);
        String header_value = line.substring(colon_pos + 1);

        // remove spaces and line breaks
        while (header_value.startsWith(" ")) {
            header_value = header_value.substring(1);
        }
        while (header_value.startsWith("\t")) {
            header_value = header_value.substring(1);
        }
        while (header_value.endsWith(" ")) {
            header_value = header_value.substring(0, header_value.length() - 1);
        }
        while (header_value.endsWith("\t")) {
            header_value = header_value.substring(0, header_value.length() - 1);
        }
        while (header_value.endsWith("\r")) {
            header_value = header_value.substring(0, header_value.length() - 1);
        }
        while (header_value.endsWith("\n")) {
            header_value = header_value.substring(0, header_value.length() - 1);
        }

        while (header_name.endsWith(" ")) {
            header_name = header_name.substring(0, header_name.length() - 1);
        }
        while (header_name.endsWith("\t")) {
            header_name = header_name.substring(0, header_name.length() - 1);
        }
        while (header_name.endsWith("\r")) {
            header_name = header_name.substring(0, header_name.length() - 1);
        }
        while (header_name.endsWith("\n")) {
            header_name = header_name.substring(0, header_name.length() - 1);
        }

        this->headers.add(header_name, header_value);
        return;
    }

    // if body
    this->message_body += line;
    return;
}

void HttpResponse::concatChunk() {
    String body = this->message_body;
    String concat_body = "";
    if (this->headers.checkTransferEncoding() == "chunked") {
        int chunk_size = 0;
        int chunk_pos = 0;
        while (chunk_pos < body.length()) {
            int crlf_pos = body.indexOf("\r", chunk_pos);
            if (crlf_pos == -1) {
                return;
            }
            chunk_size = strtol(body.substring(chunk_pos, crlf_pos).c_str(), NULL, 16);
            if (chunk_size == 0) {
                break;
            }
            concat_body += body.substring(crlf_pos + 1, crlf_pos + 1 + chunk_size);
            chunk_pos = crlf_pos + 1 + chunk_size + 1;
        }
        body = concat_body;
    }
    this->message_body = body;
}
