#include "httphelper.h"

HttpHelper::HttpHelper(Sensor &sensor, String host, int httpPort): sensor(sensor) {
    HttpHelper::host = host;
    HttpHelper::httpPort = httpPort;
};

void HttpHelper::init() {
    Serial.println("Registering sensor");
    initClient();
    
    if (connected)
    {
        String json = sensor.toJson();
        String tokenResponse = postJson(json, "weather/api/v1/sensor/" + sensor.sensorName);

        char *tokenChar = (char *)malloc(sizeof(char) * tokenResponse.length());
        tokenResponse.toCharArray(tokenChar, tokenResponse.length());

        Serial.print("Length of response: ");
        Serial.println(tokenResponse.length());

        char line[256];
        int pos = 0;
        bool startBody = false;
        for (size_t i = 0; i < tokenResponse.length(); i++)
        {
            char c = tokenChar[i];

            if (c == '\r')
            {
                line[pos] = c;

                // if line == \r\n\0
                if (strlen(line) == 3)
                {
                    startBody = true;
                }

                // random characters before body??
                if (strlen(line) > 5 && startBody)
                {
                    token = String(line);
                    token.trim();

                    break;
                }
                
                memset(line, 0 ,256);
                pos = 0;
            }
            
            line[pos] = c;
            pos++;
        }
        
        Serial.print("Token: ");
        Serial.println(token);
    }
};

String HttpHelper::postJson(String& json, String relativeUrl) {
    initClient();
    
    if (!connected)
    {
        return "connection failed";
    }
    
    Serial.println("Sending POST /" + relativeUrl + " HTTP/1.1");

    client.println("POST /" + relativeUrl + " HTTP/1.1");
    client.println("Host: " + host);
    if (token != NULL)
    {
        Serial.print("X-TOKEN: " + token);
        client.print("X-TOKEN: " + token + "\r\n");
    }
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(json.length());
    client.println();
    client.println(json);

    String returnValue = "";
    
    while(client.connected() || client.available()) 
    {
        int clientAvailable = client.available();

        if (clientAvailable)
        {
            String response = client.readString();

            if (returnValue == (returnValue += response))
            {
                break;
            }
            
            returnValue += response;
        }      
    }

    Serial.print("\r\n");
    Serial.println("Got response: ");
    Serial.println(returnValue);

    return returnValue;
};

void HttpHelper::initClient() {
    // Use WiFiClient class to create TCP connections
    if (!client.connect(host, httpPort)) {
        connected = false;
    }
    else {
        connected = true;
    }
}