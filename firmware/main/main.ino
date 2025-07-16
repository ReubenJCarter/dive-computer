#include <Adafruit_GFX.h>
// #include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
// #include <Adafruit_SPITFT_Macros.h>
// #include <gfxfont.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
/*
  Dive Computer
*/

#include <Arduino.h>
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library
#include <Adafruit_TinyUSB.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define TFT_CS 6      
#define TFT_RST 3     
#define TFT_DC 2  
#define TFT_LED 7

// initialize ILI9341 TFT library
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


//Screen 
void screenLED(bool on){
  uint32_t pinVal = on ? 1 : 0; 
  digitalWrite(7, pinVal);
}

void screenClear(){
  tft.fillScreen(ILI9341_BLACK);
}

void screenInit(){
  pinMode(7, OUTPUT);
  screenLED(true);
 
  tft.begin();
  tft.setRotation(3);
  tft.setSPISpeed(16000000UL);

  tft.setFont(&FreeSans9pt7b);
  tft.setFont(NULL);

  screenClear(); 
}




//drawing functions


void drawBatteryIndicator(int16_t x, int16_t y, int16_t width, int16_t height, int16_t percent){
  
  if(percent < 0)
    percent = 0;

  int16_t color;
  if(percent < 10){
    color = ILI9341_RED; 
  }
  else if(percent < 30){
    color = ILI9341_ORANGE; 
  }
  else {
    color = ILI9341_GREEN; 
  }

  int16_t innerGap = 2; 

  tft.fillRect(x, y, width, height, ILI9341_BLACK); 
  tft.drawRect(x, y, width, height, color);
  tft.drawRect(x+width, y+3, 5, height-6, color);

  int16_t maxW = width-innerGap*2; 
  int16_t w = ((int32_t)(maxW * percent)) / 100;  

  tft.fillRect(x+innerGap, y+innerGap, w, height-innerGap*2, color);
}

void drawButtonPressPulsate(int16_t x, int16_t y, int16_t width, int16_t height, int16_t direction, int16_t percent){
  
  if(percent < 1){
    percent = 1;
  }

  int16_t w = ((int32_t)(width * percent)) / 100;  

  if(direction == 0){
    tft.fillRect(x+w, y, width-w, height, ILI9341_BLACK); 
    tft.fillRect(x, y, w, height, ILI9341_WHITE); 
  }
  else {
    tft.fillRect(x, y, width-w, height, ILI9341_BLACK); 
    tft.fillRect(x+width-w, y, w, height, ILI9341_WHITE); 
  }
}

void drawGridLines(){
  int16_t lineThickness = 3; 
  int16_t boxH = 150; 
  tft.fillRect(145, 0, lineThickness, boxH, ILI9341_BLUE); 
  tft.fillRect(0, boxH, SCREEN_WIDTH, lineThickness, ILI9341_BLUE); 
}

void drawDepthAndTime(int64_t x, int64_t y){

  //depth
  tft.setCursor(x+30, y+10);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(6);
  tft.println("24");

  tft.setCursor(x+110, y+30);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(3);
  tft.println("M");

  //time
  tft.setCursor(x+5, y+70);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("TIME");

  tft.setCursor(x+30, y+93);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("12:42");
}

void drawStopNDL(int64_t x, int64_t y){
  //SAFTY STOP
  tft.setCursor(x+5, y+10);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("SAFTEY STOP");

  tft.setCursor(x+30, y+32);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("3:00");

  //NDL
  tft.setCursor(x+5, y+70);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("NDL");

  tft.setCursor(x+30, y+93);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("12:42");
}

void drawMODTempTime(int16_t x, int16_t y){
  //Air 
  tft.setCursor(x+5, y+30);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("AIR");

  //MOD 
  tft.setCursor(x+100, y+10);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("MOD");

  tft.setCursor(x+100, y+30);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("55");

  //temp
  tft.setCursor(x+250, y+20);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("15");

  tft.setCursor(x+280, y+20);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("C");

  //temp
  tft.setCursor(x+210, y+50);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("11:00");

  tft.setCursor(x+280, y+50);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("AM");

}

void drawJokeTC(int16_t x, int16_t y){
  tft.setCursor(x, y);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(1);
  tft.println("Terms of use: Don't be a F**CKING IDIOT");
}




// the setup function runs once when you press reset or power the board

int16_t batteryPercent = 100;

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //init the screen hardware
  screenInit(); 

  drawGridLines(); 
  drawDepthAndTime(0, 0); 
  drawStopNDL(150, 0);
  drawMODTempTime(0, 150); 
  drawJokeTC(0, 230); 
  
}

// the loop function runs over and over again forever
void loop() {
  //screen_clear(); 
  drawBatteryIndicator(SCREEN_WIDTH-30, 1, 24, 10, batteryPercent);
  //drawButtonPressPulsate(200, 100, 40, 100, 1, batteryPercent); 
  delay(100);
  batteryPercent -= 1;
}
