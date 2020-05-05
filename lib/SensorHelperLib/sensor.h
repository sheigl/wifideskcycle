#include <Arduino.h>

class Sensor {
    public:
        String sensorName;
        String sensorModel;
        unsigned long readInterval;
        Sensor(String sensorName, String sensorModel, unsigned long readInterval);
        Sensor();
        String toJson();
};