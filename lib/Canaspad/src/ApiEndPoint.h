#ifndef ApiEndPoint_h
#define ApiEndPoint_h

#include "Canaspad.h"

class CanaspadEndPoint {
  public:
    // Gotrue
    const char* auth = "/token";
    // Postgres
    const char* tube = "/tube";       // need to auth
    const char* element = "/element"; // need to auth
    //  Storage
    const char* object = "/object"; // need to auth
};

#endif
