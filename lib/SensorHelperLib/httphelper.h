#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "sensor.h"

class HttpHelper {
    public:
    Sensor sensor;
    WiFiClient client;
    bool connected;
    String postJson(String &json, String relativeUrl);
    HttpHelper(Sensor &sensor, String host, int httpPort);
    HttpHelper();
    void init();
    
    private:
    String token;
    String host;
    int httpPort;
    void initClient();
};