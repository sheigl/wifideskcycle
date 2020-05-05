#include <ESP8266WiFi.h>
#include "httpresponse.h"

class SimpleClient {
  public:
    bool isConnected = false;
    HttpResponse *request(HttpRequest *request);
    SimpleClient(char *host, int port);
    ~SimpleClient();
    private:
      char *host;
      int port;

      WiFiClient client;
      void initClient();

      HeaderCollection *createHeaders(char *method, char* host, char *relativeUrl);
};