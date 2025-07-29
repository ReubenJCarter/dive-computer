#ifndef GLOBAL_SYSTEMS_HPP
#define GLOBAL_SYSTEMS_HPP

#include "MCUSystem.hpp"
#include "SDCardSystem.hpp"
#include "BluetoothSystem.hpp"
#include "ScreenSystem.hpp"
#include "SensorSystem.hpp"
#include "CompassSystem.hpp"
#include "ButtonSystem.hpp"
#include "UISystem.hpp"

class MCUSystem; 
extern MCUSystem mcuSystem;

class SDCardSystem;
extern SDCardSystem sdCardSystem;

class BluetoothSystem;
extern BluetoothSystem bluetoothSystem;

class ScreenSystem;
extern ScreenSystem screenSystem;

class SensorSystem;
extern SensorSystem sensorSystem;

class CompassSystem;
extern CompassSystem compassSystem;

class ButtonSystem;
extern ButtonSystem buttonSystem;

class UISystem;
extern UISystem uiSystem;

#endif // GLOBAL_SYSTEMS_HPP