#include "cyclecalculations.h"
#ifndef ARD_H
#define ARD_H
#include <Arduino.h>
#endif

CycleCalculations::CycleCalculations() {
    lastTriggerTime = millis();
}

bool CycleCalculations::hasStoppedPedaling()
{
      //  if triggerInterval > 3 seconds, you've probably stopped pedalling
    long now = millis();

    bool returnValue = now - currentTriggerTime >= 3000;

    if (returnValue) {
        cadence = 0;
        lastSpeed = currentSpeed;
        currentSpeed = 0;
    }

    return returnValue;
}

ICACHE_RAM_ATTR void CycleCalculations::onCycleInterrupt(int value)
{
  //  Read the value of the trigger
  triggerValue = value;

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