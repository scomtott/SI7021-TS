#ifndef SI7021_TS_h
#define SI7021_TS_h
#endif

class SI7021_TS
{
    public:
        SI7021_TS();
        bool begin();
        bool sensorExists();
        int getHumidityPercent();
        float getTemperatureDegrees();
    private:
        long _makeMeasurement(int cmd);
};