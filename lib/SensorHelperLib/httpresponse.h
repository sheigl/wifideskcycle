#include "httprequest.h"
class HttpResponse {
  public:
    char *body;
    HeaderCollection *headers;
    HttpRequest *request;

    HttpResponse();
    ~HttpResponse();
};