#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "sensordata.h"
#include "httphelper.h"
#include "simpleclient.h"

#define USE_SERIAL Serial

#ifndef STASSID
#define STASSID ""
#define STAPSK ""
#endif

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
float lastSpeed = 0.0;

const long delayTime = 2000;
char* host = "www.stevenheigl.com";
const String url = "weather/api/v1/generic";

Sensor sensor = Sensor("WiFiDeskCycle", "WDC001", delayTime);
HttpHelper helper = HttpHelper(sensor, host, 80);

void setup()
{

  USE_SERIAL.begin(9600);

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    USE_SERIAL.print(".");
  }

  USE_SERIAL.println("");
  USE_SERIAL.print("Connected! IP address: ");
  USE_SERIAL.println(WiFi.localIP());

  pinMode(trigger, INPUT_PULLUP);

  //  set the initial value so that it isn't 0
  lastTriggerTime = millis();
  attachInterrupt(digitalPinToInterrupt(trigger), cycleInterrupt, RISING);

  helper.init();
}

ICACHE_RAM_ATTR void cycleInterrupt()
{
  //  Read the value of the trigger
  triggerValue = digitalRead(trigger);

  //  measure the time since the last
  currentTriggerTime = millis();
  triggerInterval = currentTriggerTime - lastTriggerTime;

  //  State has changed so update the last state var
  lastTriggerValue = triggerValue;

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
  lastSpeed = currentSpeed;
  currentSpeed = inchesPerHour / inchesPerMile;
}

void loop()
{
  //  if triggerInterval > 3 seconds, you've probably stopped pedalling
  long now = millis();

  if (now - currentTriggerTime >= 3000)
  {
    cadence = 0;
    lastSpeed = currentSpeed;
    currentSpeed = 0;
  }
  else 
  {
    // wait for WiFi connection
    if ((WiFi.status() == WL_CONNECTED))
    {

    SensorData speed = SensorData(
        currentSpeed,
        "MPH"
      );

    SensorData cadenceData = SensorData(
        cadence,
        "RPM"
      );

    SensorData resistance = SensorData(
        6,
        "RES"
      );

      speed.sensorName = sensor.sensorName;
      cadenceData.sensorName = sensor.sensorName;
      resistance.sensorName = sensor.sensorName;

      String json = "[" + speed.toJson() + "," + cadenceData.toJson() + "," + resistance.toJson() + "]";
      Serial.print("JSON: ");
      Serial.println(json);

      helper.postJson(json, url);

      delay(delayTime);
    }
  }

  delay(10);
}