#include "Arduino.h"
#include "SI7021_TS.h"
#include <Wire.h>

// Default I2C address for SI7021 is 0x40
#define I2C_ADDR 0x40

// I2C commands
int RH_READ           = 0xF5;
int TEMP_READ         = 0xF3;

bool _si_exists = false;

SI7021_TS::SI7021_TS() {
}

bool SI7021_TS::begin() {
    Wire.begin();
    Wire.beginTransmission(I2C_ADDR);
    if (Wire.endTransmission() == 0) {
        _si_exists = true;
    }
    return _si_exists;
}

bool SI7021_TS::sensorExists() {
    return _si_exists;
}

int SI7021_TS::getHumidityPercent() {
    long humidity_reading = _makeMeasurement(RH_READ);

    int humidity = (int)(((125 * humidity_reading) / 65536.0 ) - 6);

    // Percentage humidity value may read as slightly above 100 so clamp
    // it to 100.
    return (humidity > 100 ? 100 : humidity);
}

float SI7021_TS::getTemperatureDegrees() {
    long temperature_reading = _makeMeasurement(TEMP_READ);

    return (((175.72 * temperature_reading) / 65536.0) - 46.85);
}

long SI7021_TS::_makeMeasurement(int cmd)
{
    byte data[2];

    // Start I2C transmission
    Wire.beginTransmission(I2C_ADDR);

    // Send measurement command
    Wire.write(cmd);

    // Stop I2C transmission
    Wire.endTransmission();
    delay(200);

    // Request 2 bytes of data
    Wire.requestFrom(I2C_ADDR, 2);

    // Read 2 bytes of data
    // humidity msb, humidity lsb 
    if(Wire.available() == 2)
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }

    // Combine two elements to form 16 bit word
    return (long)data[0] << 8 | data[1];
}
