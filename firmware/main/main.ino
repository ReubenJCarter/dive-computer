#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define _TIMERINTERRUPT_LOGLEVEL_     4               //From 0 - 4 No idea what this means
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default
#include <ESP32Time.h>

#include <Arduino.h>
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library

#include <SPI.h>
#include <SD.h>
#include "driver/spi_common.h"  // Needed for SPI2_HOST, SPI3_HOST
#include <ArduinoJson.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define BUT0_APIN A6
#define BUT1_APIN A7

#define TFT_CS 2      
#define TFT_RST 3     
#define TFT_DC 4  
#define TFT_LED 5

#define SD_CS 20

#define SPI_MISO 12
#define SPI_MOSI 11
#define SPI_CLK 13

SPIClass SPI_CUSTOM(SPI2_HOST);

void initSPI(){
  SPI_CUSTOM.begin(SPI_CLK, SPI_MISO, SPI_MOSI, -1); 
}

//sd Card

#define CONFIG_FILE_NAME "/config.json"
JsonDocument config; 

void initSD(){

  //fire the sd card up
  Serial.println("Init SD..."); 
  
  int sdInited = SD.begin(SD_CS, SPI_CUSTOM); 
  
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


//Screen
Adafruit_ILI9341* tft; 


void screenLED(bool on){
  uint32_t pinVal = on ? 1 : 0; 
  digitalWrite(TFT_LED, pinVal);
}

void screenClear(){
  tft->fillScreen(ILI9341_BLACK);
}

void screenInit(){

  tft = new Adafruit_ILI9341(&SPI_CUSTOM, TFT_DC, TFT_CS, TFT_RST); 

  pinMode(TFT_LED, OUTPUT);
  screenLED(true);
 
  tft->begin();
  tft->setRotation(3);
  tft->setSPISpeed(16000000UL);

  tft->setFont(&FreeSans9pt7b);
  tft->setFont(NULL);

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

  tft->fillRect(x, y, width, height, ILI9341_BLACK); 
  tft->drawRect(x, y, width, height, color);
  tft->drawRect(x+width, y+3, 5, height-6, color);

  int16_t maxW = width-innerGap*2; 
  int16_t w = ((int32_t)(maxW * percent)) / 100;  

  tft->fillRect(x+innerGap, y+innerGap, w, height-innerGap*2, color);
}

void drawButtonPressPulsate(int16_t x, int16_t y, int16_t width, int16_t height, int16_t direction, int16_t percent){
  
  if(percent < 1){
    percent = 1;
  }

  int16_t w = ((int32_t)(width * percent)) / 100;  

  if(direction == 0){
    tft->fillRect(x+w, y, width-w, height, ILI9341_BLACK); 
    tft->fillRect(x, y, w, height, ILI9341_WHITE); 
  }
  else {
    tft->fillRect(x, y, width-w, height, ILI9341_BLACK); 
    tft->fillRect(x+width-w, y, w, height, ILI9341_WHITE); 
  }
}

void drawGridLines(){
  int16_t lineThickness = 3; 
  int16_t boxH = 150; 
  tft->fillRect(145, 0, lineThickness, boxH, ILI9341_BLUE); 
  tft->fillRect(0, boxH, SCREEN_WIDTH, lineThickness, ILI9341_BLUE); 
}

void drawDepthAndTime(int64_t x, int64_t y){

  //depth
  tft->setCursor(x+30, y+10);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(6);
  tft->println("24");

  tft->setCursor(x+110, y+30);
  tft->setTextColor(ILI9341_BLUE);  
  tft->setTextSize(3);
  tft->println("M");

  //time
  tft->setCursor(x+10, y+70);
  tft->setTextColor(ILI9341_BLUE);  
  tft->setTextSize(2);
  tft->println("TIME");

  tft->setCursor(x+30, y+93);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(3);
  tft->println("12:42");
}

void drawStopNDL(int64_t x, int64_t y){
  //SAFTY STOP
  tft->setCursor(x+5, y+10);
  tft->setTextColor(ILI9341_BLUE);  
  tft->setTextSize(2);
  tft->println("SAFTEY STOP");

  tft->setCursor(x+30, y+32);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(3);
  tft->println("3:00");

  //NDL
  tft->setCursor(x+5, y+70);
  tft->setTextColor(ILI9341_BLUE);  
  tft->setTextSize(2);
  tft->println("NDL");

  tft->setCursor(x+30, y+93);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(3);
  tft->println("12:42");
}

void drawMODTempTime(int16_t x, int16_t y){
  //Air 
  tft->setCursor(x+5, y+30);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(3);
  tft->println("AIR");

  //MOD 
  tft->setCursor(x+100, y+10);
  tft->setTextColor(ILI9341_BLUE);  
  tft->setTextSize(2);
  tft->println("MOD");

  tft->setCursor(x+100, y+30);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(3);
  tft->println("55");

  //temp
  tft->setCursor(x+250, y+20);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(2);
  tft->println("15");

  tft->setCursor(x+280, y+20);
  tft->setTextColor(ILI9341_BLUE);  
  tft->setTextSize(2);
  tft->println("C");

  //temp
  tft->setCursor(x+210, y+50);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(2);
  tft->println("11:00");

  tft->setCursor(x+280, y+50);
  tft->setTextColor(ILI9341_BLUE);  
  tft->setTextSize(2);
  tft->println("AM");

}

void drawJokeTC(int16_t x, int16_t y){
  tft->setCursor(x, y);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(1);
  tft->println("Terms of use: Don't be a F**CKING IDIOT");
}

void drawTextWithBackground(int16_t x, int16_t y, uint8_t size, uint16_t color, uint16_t backgroundColor, const char* text){
  tft->setCursor(x, y);
  tft->setTextColor(ILI9341_WHITE);  
  tft->setTextSize(1);
  int16_t x1;
  int16_t y1;
  uint16_t w;
  uint16_t h;
  tft->getTextBounds(text, x, y, &x1, &y1, &w, &h); 
  tft->fillRect(x, y, w, h, backgroundColor); 
  tft->print(text); 
}


//Time tracking
//ESP32Time rtc(offset);


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


//Set up hardware timer ISR 
hw_timer_t *timer0 = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;

#define TIMER0_INTERVAL_MS 300
bool ledState = false; 
void IRAM_ATTR onTimer0() {

  portENTER_CRITICAL_ISR(&timerMux0);
  ledState = !ledState;
  if(ledState){
    digitalWrite(LED_RED, 1);
  }
  else {
    digitalWrite(LED_RED, 0);
  }
  //digitalWrite(LED_PIN, ledState);
  portEXIT_CRITICAL_ISR(&timerMux0);
}

void initHardwareTimers(){
  // Initialize the timer
  // timerBegin(timer number [0-3], prescaler, countUp)
  timer0 = timerBegin(0, 8000, true); // 8000 prescaler = 1us tick (80MHz/8000)

  // Attach the interrupt
  // timerAttachInterrupt(timer, ISR, edge)
  timerAttachInterrupt(timer0, &onTimer0, true);

  // Set the timer alarm
  // timerAlarmWrite(timer, ticks, auto-reload)
  timerAlarmWrite(timer0, 5000, true); // 5000 us = 0.5s

  // Enable the alarm
  timerAlarmEnable(timer0);
}


// the setup function runs once when you press reset or power the board

float batteryPercent = 100;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
   
  Serial.println("ILI9341 Test!"); 
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_RED, OUTPUT);

  //init the screen hardware

  initSPI();
  initSD(); 
  screenInit(); 

  initHardwareTimers(); 

  drawGridLines(); 
  drawDepthAndTime(0, 0); 
  drawStopNDL(150, 0);
  drawMODTempTime(0, 150); 
  drawJokeTC(30, 230); 

  setupBluetooth(); 
  
}


// the loop function runs over and over again forever
void loop() {
  drawBatteryIndicator(SCREEN_WIDTH-30, 1, 24, 10, (uint16_t)batteryPercent);

  updateButtons(); 

  uint16_t buttonFlashWidth = 15; 
  uint16_t buttonFlashHeight = 30; 
  tft->fillRect(SCREEN_WIDTH-buttonFlashWidth, SCREEN_HEIGHT-buttonFlashHeight, buttonFlashWidth, buttonFlashHeight, but0State ? ILI9341_WHITE : ILI9341_BLACK);
  tft->fillRect(0, SCREEN_HEIGHT-buttonFlashHeight, buttonFlashWidth, buttonFlashHeight, but1State ? ILI9341_WHITE : ILI9341_BLACK);

  delay(2);
  batteryPercent -= 0.01;
}
