#ifndef SENSOR_SYSTEM_HPP
#define SENSOR_SYSTEM_HPP

#include <MS5837.h>

class SensorSystem {
private:
    MS5837 pressureSensor;

public:
    SensorSystem();
    void begin();
    void capture(); 
    float getTemperature(); 
    float getPressure();
    float getDepth();
    float getAltitude();
};

#endif // SENSOR_SYSTEM_HPP