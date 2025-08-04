#ifndef SD_CARD_SYSTEM_HPP
#define SD_CARD_SYSTEM_HPP

#define SD_CS 20
#define CONFIG_FILE_NAME "/config.json"

#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>

class SDCardSystem {

public:
    JsonDocument config; 
    
    SDCardSystem();
    void begin();
    void writeConfig(); 
}; 

#endif // SD_CARD_SYSTEM_HPP