#include "headercollection.h"

class HttpRequest {
  public:
    HeaderCollection *headers;
    char *url;
    char *method;
    char *body;
    char *token;
    HttpRequest(char *method, char *url);
    ~HttpRequest();
};