/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_IP "192.168.1.42"

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif


//timer setup for timer0.
//For arduino uno or any board with ATMEL 328/168.. diecimila, duemilanove, lilypad, nano, mini...

//this code will enable the timer0 interrupt.
//timer0 will interrupt at 2kHz

//  useful maths constants
const float pi = 3.14159265;
const float inchesPerMile = 63360;

//  virtual bike wheel size in inches
const int wheelSize = 26;

//  virtual bike gear ration, nonesense for now
const float gearRatio = 2.75;

//  virtual wheel circumference
const float wheelCircumference = wheelSize * pi;

//storage variables

//  lastTriggerTimer will contain the last time that the pedals completed a revolution
//  measured in milliseconds since the Arduino started, it will reset around every 50 days
//  http://arduino.cc/en/Reference/Millis
long lastTriggerTime = 0;

//  currentTriggerTime is used to hold the current time to calculate the timespane between revolutions
long currentTriggerTime = 0;

//  holds the timespan between triggers
long triggerInterval = 0;

//  lastTriggerValue does what it says on the tin
//  using this we can prevent issues when the switch is detected as on for a while
int lastTriggerValue = 0;

//  triggerValue will hold the current value of the trigger
int triggerValue = 0;

//  Pin the switch it connected to
int trigger = 13;

//  current cadence
float cadence = 0;

//  current speed
float currentSpeed = 0;


void setup() {

  USE_SERIAL.begin(115200);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    USE_SERIAL.print(".");
  }
  USE_SERIAL.println("");
  USE_SERIAL.print("Connected! IP address: ");
  USE_SERIAL.println(WiFi.localIP());

  //  Set up the trigger input
  pinMode(trigger, INPUT);           // set pin to input
  digitalWrite(trigger, HIGH);       // turn on pullup resistors
  
  //  set the initial value so that it isn't 0
  lastTriggerTime = millis(); 

   attachInterrupt(digitalPinToInterrupt(trigger), ISR, mode);

}

void ISR() {
  //timer1 interrupt 2kHz, calculates cadence
  
  //  Read the value of the trigger
  triggerValue = digitalRead(trigger);
  
  //  pullup resistors activate so value is inverted
  //  normalise it to 0 for off and 1 for on
  triggerValue = triggerValue == 0 ? 1 : 0;
  
  //  measure the time since the last
  currentTriggerTime = millis();
  triggerInterval = currentTriggerTime - lastTriggerTime;
  
  //  if triggerInterval > 3 seconds, you've probably stopped pedalling
  if(triggerInterval >= 3000)
  {
    cadence = 0;
    currentSpeed = 0;
  }
  
  //  If the pin has changed state, reset the counter
  if(lastTriggerValue != triggerValue)
  {
    //  State has changed so update the last state var
    lastTriggerValue = triggerValue;
    
    if(triggerValue == 1)
    {
      //  If 1 then we have completed a revolution
      lastTriggerTime = currentTriggerTime;
            
      //  cadence = 60000ms / interval
      cadence = 60000 / triggerInterval;
            
      //  how many times as the rear wheel turned?
      //  assume the rear wheel turns 4 times per pedel revolution
      //  we'll make this accurate to my bike at some point, just using it as a magic number for now
      
      //  rph = revolutions per hour of the pedals
      float rph = cadence * 60;
      float wheelRph = rph * gearRatio;
      
      //  speed in inches per hour
      float inchesPerHour = wheelCircumference * wheelRph;
      currentSpeed = inchesPerHour / inchesPerMile;      
    }
  }
}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/postplain/"); //HTTP
    http.addHeader("Content-Type", "application/json");

    USE_SERIAL.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST("{\"hello\":\"world\"}");

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        USE_SERIAL.println("received payload:\n<<");
        USE_SERIAL.println(payload);
        USE_SERIAL.println(">>");
      }
    } else {
      USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}
