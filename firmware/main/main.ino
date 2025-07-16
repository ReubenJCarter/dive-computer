#include <Adafruit_GFX.h>
// #include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
// #include <Adafruit_SPITFT_Macros.h>
// #include <gfxfont.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

/*
  Dive Computer
*/

#include <Arduino.h>
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define BUT0_APIN A6
#define BUT1_APIN A7

#define TFT_CS 2      
#define TFT_RST 3     
#define TFT_DC 4  
#define TFT_LED 5


//bluetooth
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic;
bool deviceConnected = false;

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "abcd1234-ab12-cd34-ef56-1234567890ab"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setupBluetooth() {
  BLEDevice::init("Dive Logic - The Wrasse V1");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setValue("Hello BLE");
  pService->start();
  pServer->getAdvertising()->start();
}


// initialize ILI9341 TFT library
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//Screen 
void screenLED(bool on){
  uint32_t pinVal = on ? 1 : 0; 
  digitalWrite(TFT_LED, pinVal);
}

void screenClear(){
  tft.fillScreen(ILI9341_BLACK);
}

void screenInit(){
  pinMode(TFT_LED, OUTPUT);
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
  tft.setCursor(x+10, y+70);
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

void drawTextWithBackground(int16_t x, int16_t y, uint8_t size, uint16_t color, uint16_t backgroundColor, const char* text){
  tft.setCursor(x, y);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(1);
  int16_t x1;
  int16_t y1;
  uint16_t w;
  uint16_t h;
  tft.getTextBounds(text, x, y, &x1, &y1, &w, &h); 
  tft.fillRect(x, y, w, h, backgroundColor); 
  tft.print(text); 
}


//handle buttons
unsigned long butDebounceDelay = 50; 

bool but0LastReading = false; 
bool but0State = false; 
unsigned long but0Time = 0;

bool but1LastReading = false; 
bool but1State = false; 
unsigned long but1Time = 0;

void updateButtons(){
  unsigned int but0a = analogRead(BUT0_APIN);
  bool but0Reading = but0a > 900 ? false : true; 

  if(but0Reading != but0LastReading){
    but0Time = millis(); 
  }
  but0LastReading = but0Reading; 

  if ((millis() - but0Time) > butDebounceDelay) {
    if(but0Reading != but0State){
      but0State = but0Reading; 
    }
  }

  unsigned int but1a = analogRead(BUT1_APIN);
  bool but1Reading = but1a > 900 ? false : true; 

  if(but1Reading != but1LastReading){
    but1Time = millis(); 
  }
  but1LastReading = but1Reading; 

  if ((millis() - but1Time) > butDebounceDelay) {
    if(but1Reading != but1State){
      but1State = but1Reading; 
    }
  }
}


// the setup function runs once when you press reset or power the board

float batteryPercent = 100;

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
  drawJokeTC(30, 230); 

  setupBluetooth(); 
  
}


// the loop function runs over and over again forever
void loop() {
  //screen_clear(); 
  drawBatteryIndicator(SCREEN_WIDTH-30, 1, 24, 10, (uint16_t)batteryPercent);
  //drawButtonPressPulsate(200, 100, 40, 100, 1, batteryPercent); 

  updateButtons(); 

  uint16_t buttonFlashWidth = 15; 
  uint16_t buttonFlashHeight = 30; 
  tft.fillRect(SCREEN_WIDTH-buttonFlashWidth, SCREEN_HEIGHT-buttonFlashHeight, buttonFlashWidth, buttonFlashHeight, but0State ? ILI9341_WHITE : ILI9341_BLACK);
  tft.fillRect(0, SCREEN_HEIGHT-buttonFlashHeight, buttonFlashWidth, buttonFlashHeight, but1State ? ILI9341_WHITE : ILI9341_BLACK);

  delay(2);
  batteryPercent -= 0.01;
}
