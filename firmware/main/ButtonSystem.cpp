#include "ButtonSystem.hpp"

#include <Arduino.h>

ButtonSystem::ButtonSystem() {
   
}

void ButtonSystem::begin() {
    but0LastReading = false; 
    but0State = false; 
    but0Time = 0;

    but1LastReading = false; 
    but1State = false; 
    but1Time = 0;

    pinMode(BUT0_APIN, INPUT);
    pinMode(BUT1_APIN, INPUT);
}

void ButtonSystem::update() {

    unsigned int but0a = analogRead(BUT0_APIN);
    bool but0Reading = but0a > BUT_THRESHOLD ? false : true; 
  
    if(but0Reading != but0LastReading){
      but0Time = millis(); 
    }
    but0LastReading = but0Reading; 
  
    if ((millis() - but0Time) > BUT_DEBOUNCE_DELAY) {
      if(but0Reading != but0State){
        but0State = but0Reading; 
      }
    }
  
    unsigned int but1a = analogRead(BUT1_APIN);
    bool but1Reading = but1a > BUT_THRESHOLD ? false : true; 
  
    if(but1Reading != but1LastReading){
      but1Time = millis(); 
    }
    but1LastReading = but1Reading; 
  
    if ((millis() - but1Time) > BUT_DEBOUNCE_DELAY) {
      if(but1Reading != but1State){
        but1State = but1Reading; 
      }
    }
}