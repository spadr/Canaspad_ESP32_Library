#ifndef ApiEndPoint_h
#define ApiEndPoint_h

#include "HttpStatusCodes.h"
#include <Arduino.h>

typedef String url_t;

class ApiEndPoint {
  public:
    // Gotrue
    url_t auth_url = "/token";
    // Postgres
    url_t tube_url = "/tube";                    // need to auth
    url_t element_url = "/element";              // need to auth
    url_t fresh_element_url = "/fresh_elements"; // need to auth
};

#endif
