#include "DiveComputer.hpp"

#include "globalSystems.hpp"

DiveComputer::DiveComputer() {

}   

void DiveComputer::begin() {
    mcuSystem.begin();
    sdCardSystem.begin();
    bluetoothSystem.begin();
    screenSystem.begin();
    sensorSystem.begin();
    compassSystem.begin();
    buttonSystem.begin();
    uiSystem.begin();
}

void DiveComputer::update() {

}