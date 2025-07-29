#include "SDCardSystem.hpp"

#include "globalSystems.hpp"

SDCardSystem::SDCardSystem() {

}

void SDCardSystem::begin() {
    //fire the sd card up (SD card needs to be formatted with FAT32)
    Serial.println("Init SD..."); 
    
    int sdInited = SD.begin(SD_CS, mcuSystem.spiClass); 
    
    if(!sdInited){
        Serial.println("Could not Init SD"); 
        return; 
    }

    //check if config file exists. If it does not, make it with default config
    bool configExists = SD.exists(CONFIG_FILE_NAME) ? true : false; 
    if(!configExists){
        Serial.println("Making config file"); 
        
        JsonDocument defaultConfig;
        defaultConfig["deviceModel"] = "wrasse";

        File defaultConfigFile = SD.open(CONFIG_FILE_NAME, FILE_WRITE);
        if(defaultConfigFile){
        serializeJson(defaultConfig, defaultConfigFile);
        }
        else {
        Serial.println("Could not open the config file for write");
        }
        defaultConfigFile.close();  
    }

    //Read in the JSON config file
    Serial.println("Reading config file..."); 
    File configFile = SD.open(CONFIG_FILE_NAME, FILE_READ);
    if(configFile){
        configFile.seek(0); 
        String json = configFile.readString(); 
        DeserializationError error = deserializeJson(config, json);
        configFile.close(); 
    }
    else {
        Serial.println("Could not open the config file for reading");
    }
}