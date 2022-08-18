#ifndef ApiEndPoint_h
#define ApiEndPoint_h

#include "Type.h"

class ApiEndPoint
{
public:
    // Gotrue
    url_t auth = "/token";
    // Postgres
    url_t tube = "/tube";                    // need to auth
    url_t element = "/element";              // need to auth
    url_t fresh_element = "/fresh_elements"; // need to auth
};

#endif
