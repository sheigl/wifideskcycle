#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "sensordata.h"
#include "httphelper.h"
#include "simpleclient.h"
#include "cyclecalculations.h"
#include "wirelessinfo.h"

#define USE_SERIAL Serial

const int triggerPin = 13;
const long delayTime = 2000;
const String host = "www.stevenheigl.com";
const String url = "weather/api/v1/generic";

Sensor sensor = Sensor("WiFiDeskCycle", "WDC001", delayTime);
HttpHelper helper = HttpHelper(sensor, host, 80);

CycleCalculations cycleCalc = CycleCalculations();
WirelessInfo wirelessInfo = WirelessInfo();

void setup()
{
  ICACHE_RAM_ATTR void cycleInterrupt();

  USE_SERIAL.begin(9600);

  WiFi.begin(wirelessInfo.ssid, wirelessInfo.password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    USE_SERIAL.print(".");
  }

  USE_SERIAL.println("");
  USE_SERIAL.print("Connected! IP address: ");
  USE_SERIAL.println(WiFi.localIP());

  pinMode(triggerPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(triggerPin), cycleInterrupt, RISING);

  helper.init();
}

ICACHE_RAM_ATTR void cycleInterrupt() 
{
  cycleCalc.onCycleInterrupt(digitalRead(triggerPin));
}


void loop()
{
  if (!cycleCalc.hasStoppedPedaling()) 
  {
    // wait for WiFi connection
    if ((WiFi.status() == WL_CONNECTED))
    {

      SensorData speed = SensorData(
          cycleCalc.currentSpeed,
          "MPH"
        );

      SensorData cadenceData = SensorData(
          cycleCalc.cadence,
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