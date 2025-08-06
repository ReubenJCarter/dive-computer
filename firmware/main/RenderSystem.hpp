#ifndef SCREEN_SYSTEM_HPP
#define SCREEN_SYSTEM_HPP

#define TFT_CS 2      
#define TFT_RST 3     
#define TFT_DC 4  
#define TFT_LED 5

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <Arduino.h>

#include <Adafruit_ILI9341.h> 

class ScreenSystem {
private:
    Adafruit_ILI9341* tft; 

public:
    ScreenSystem(); 
    void begin();
    void setScreenBacklight(bool on);
    void clearScreen();

    void drawBatteryIndicator(int16_t x, int16_t y, int16_t width, int16_t height, int16_t percent); 
    void drawButtonPressPulsate(int16_t x, int16_t y, int16_t width, int16_t height, int16_t direction, int16_t percent); 
    void drawGridLines(); 
    void drawDepthAndTime(int64_t x, int64_t y); 
    void drawStopNDL(int64_t x, int64_t y); 
    void drawMODTempTime(int16_t x, int16_t y); 
    void drawJokeTC(int16_t x, int16_t y); 
    void drawTextWithBackground(int16_t x, int16_t y, uint8_t size, uint16_t color, uint16_t backgroundColor, const char* text); 
}; 

#endif // SCREEN_SYSTEM_HPP