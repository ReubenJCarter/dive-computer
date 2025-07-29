#include "SensorSystem.hpp"

SensorSystem::SensorSystem() {
    
}

void SensorSystem::begin() {
    // Initialize pressure sensor
    // Returns true if initialization was successful
    // We can't continue with the rest of the program unless we can initialize the sensor
    delay(500); //wait for a bit for system to stabilize
    if(!pressureSensor.init()) {
        Serial.println("Pressure sensor init failed!");
        return; 
    }

    pressureSensor.setFluidDensity(1029); // kg/m^3 (997 freshwater, 1029 for seawater).... TODO get this from the config file
}

void SensorSystem::capture() {
    pressureSensor.read(); 
}

float SensorSystem::getTemperature() {
    return pressureSensor.temperature();
}

float SensorSystem::getPressure() {
    return pressureSensor.pressure();
}

float SensorSystem::getDepth() {
    return pressureSensor.depth();  
}

float SensorSystem::getAltitude() {
    return pressureSensor.altitude();
}

