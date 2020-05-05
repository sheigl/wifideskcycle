#include "sensordata.h"

SensorData::SensorData(float measurement, String unitOfMeasure) {
    SensorData::measurement = measurement;
    SensorData::unitOfMeasure = unitOfMeasure;
}

String SensorData::toJson() {
    String returnValue = "{";
    return returnValue + "\"measurement\":" + measurement + ","
    + "\"unitOfMeasure\":\"" + unitOfMeasure + "\","
    + "\"sensorName\":\"" + sensorName + "\""
    + "}";
};