#ifndef SD_CARD_SYSTEM_HPP
#define SD_CARD_SYSTEM_HPP

#define SD_CS 20
#define CONFIG_FILE_NAME "/config.json"

#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>

class SDCardSystem {
private:
    JsonDocument config;    

public:
    SDCardSystem();
    void begin();
}; 

#endif // SD_CARD_SYSTEM_HPP