#include <Arduino.h>

class SensorData 
{
    public:
        float measurement;
        String unitOfMeasure;
        String sensorName;
        SensorData(float measurement, String unitOfMeasure);  
        String toJson();
};