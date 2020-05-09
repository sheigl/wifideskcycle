class CycleCalculations {
    public:
        CycleCalculations();
        void onCycleInterrupt(int value);
        bool hasStoppedPedaling();

        //  current cadence
        float cadence = 0;

        //  current speed
        float currentSpeed = 0;
    private:
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

        // the last speed recorded
        float lastSpeed = 0.0;
};