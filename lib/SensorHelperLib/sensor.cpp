#include "sensor.h"

Sensor::Sensor(String sensorName, String sensorModel, unsigned long readInterval) {
    Sensor::sensorName = sensorName;
    Sensor::sensorModel = sensorModel;
    Sensor::readInterval = readInterval;
};

String Sensor::toJson() {
    String returnValue = "{";
    return returnValue + "\"sensorName\":\"" + sensorName + "\","
    + "\"sensorModel\":\"" + sensorModel + "\","
    + "\"readInterval\":" + readInterval
    + "}";
};