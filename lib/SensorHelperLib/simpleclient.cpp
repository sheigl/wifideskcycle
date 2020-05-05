#include "simpleclient.h"

SimpleClient::SimpleClient(char *host, int port) {
    Serial.println("Creating SimpleClient()");

    int size = sizeof(char) * 2048;

    Serial.print("Allocating ");
    Serial.print(size);
    Serial.println(" bytes...");

    this->host = (char *)malloc(size);
    
    strcpy(this->host, host);
    this->port = port;
};

SimpleClient::~SimpleClient() {
    Serial.printf("Deconstructing SimpleClient(%s, %n)", this->host, this->port);
    
    if (this->host)
    {
        free(this->host);
    }
};

HttpResponse *SimpleClient::request(HttpRequest *request) {
    Serial.println("Entering request()");

    this->initClient();

    HeaderCollection *defaultHeaders = createHeaders(request->method, this->host, request->url);
    
    Serial.printf("POST /%s HTTP/1.1\r\n", request->url);
    client.printf("POST /%s HTTP/1.1\r\n", request->url);

    Serial.println("Sendng headers...");
    int len = defaultHeaders->length();
    
    for (size_t i = 0; i < len; i++)
    {
        KeyValuePair *kvp = defaultHeaders->getHeader(i);
        Serial.print(kvp->getKey());
        Serial.print(":");
        Serial.println(kvp->getValue());

        if (request->token != NULL)
        {
            client.print("X-TOKEN: ");
            client.println(request->token);
        }
        
        Serial.println("Sendng content...");
        client.print("Content-Length: ");
        client.println(strlen(request->body));
        client.println();
        client.println(request->body); 
    }
    
    Serial.println("Reading response...");

    HttpResponse *res = (HttpResponse *)malloc(sizeof(HttpResponse) * 1);

    char **responseBody = (char **)malloc(sizeof(char *) * 50);
    int index = 0;

    while(client.connected() || client.available()) 
    {
        int clientAvailable = client.available();

        if (clientAvailable)
        {        
            int charIndex = 0;
            int size = sizeof(char) * 50;   
            char c;
            char *line = (char *)malloc(size);
            while ((c = client.read()) != '\r')
            {
                if (charIndex > sizeof(char) * size)
                {
                    realloc(line, charIndex + 1);
                }

                line[charIndex] = c;
                charIndex++;
            }

            client.read(); // remove \n
            line[charIndex + 1] = '\0';
            responseBody[index] = line;

            //Serial.println(client.readStringUntil('\r'));
            index++;
        }      
    }

    return res;
};

HeaderCollection *SimpleClient::createHeaders(char *method, char *host, char *relativeUrl) {
    Serial.println("Entering createHeaders()");
    HeaderCollection *headers = (HeaderCollection *)malloc(sizeof(HeaderCollection) * 1);

    char *hostKey = (char*)malloc(sizeof(char) * sizeof("Host"));
    strcpy(hostKey, "Host");

    char *ccKey = (char*)malloc(sizeof(char) * sizeof("Cache-Control"));
    strcpy(ccKey, "Cache-Control");

    char *cc = (char*)malloc(sizeof(char) * sizeof("no-cache"));
    strcpy(cc, "no-cache");

    char *ctKey = (char*)malloc(sizeof(char) * sizeof("Content-Type"));
    strcpy(ctKey, "Content-Type");

    char *ct = (char*)malloc(sizeof(char) * sizeof("application/json"));
    strcpy(ct, "application/json");

    headers->addHeader(hostKey, host);
    headers->addHeader(ccKey, cc);
    headers->addHeader(ctKey, ct);

  return headers;
};

void SimpleClient::initClient() {
    Serial.println("Entering initClient()");
    // Use WiFiClient class to create TCP connections
    if (!client.connect(host, port)) {
        isConnected = false;
    }
    else {
        isConnected = true;
    }
}

