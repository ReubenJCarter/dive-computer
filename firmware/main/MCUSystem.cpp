#include "MCUSystem.hpp"

#include "globalSystems.hpp"

volatile uint16_t MCUSystem::timer0Counter = 0; // Initialize static counter for timer0 
volatile bool MCUSystem::timerFlag100ms = false;
volatile bool MCUSystem::timerFlag500ms = false;
volatile bool MCUSystem::timerFlag1000ms = false;
portMUX_TYPE MCUSystem::timerMux0 = portMUX_INITIALIZER_UNLOCKED; 

MCUSystem::MCUSystem() : spiClass(SPI2_HOST) {
    
}

void MCUSystem::begin() {
    spiClass.begin(SPI_CLK, SPI_MISO, SPI_MOSI, -1); 
    Wire.begin();
    //Wire.setClock(400000);
}

void IRAM_ATTR MCUSystem::timer0ISR() {
    portENTER_CRITICAL_ISR(&timerMux0);

    timer0Counter++; 

    mcuSystem.timerFlag100ms = true;

    if (timer0Counter % 5 == 0) { //500ms
        mcuSystem.timerFlag500ms = true;
    }

    if (timer0Counter % 10 == 0) { //1000ms
        mcuSystem.timerFlag1000ms = true;
    }

    // Reset the counter to prevent overflow
    if (timer0Counter >= 65535) {
        timer0Counter = 0;
    }
    
    portEXIT_CRITICAL_ISR(&timerMux0);
}

void MCUSystem::initHardwareTimers(){
  
    // Initialize the timer0: timerBegin(timer number [0-3], prescaler, countUp)
    timer0 = timerBegin(0, 8000, true); // 8000 prescaler = 0.1ms tick (80MHz/8000)
  
    // Attach the interrupt : timerAttachInterrupt(timer, ISR, edge)
    timerAttachInterrupt(timer0, &MCUSystem::timer0ISR, true);
   
    // Set the timer alarm: timerAlarmWrite(timer, ticks, auto-reload)
    timerAlarmWrite(timer0, TIMER0_PERIOD_MS * 10, true); //So the ISR fires every 100ms 
  
    // Enable the alarm
    timerAlarmEnable(timer0);
}

void MCUSystem::resetTimers() {
    portENTER_CRITICAL(&timerMux0);

    timer0Counter = 0;
    timerFlag100ms = false;
    timerFlag500ms = false;
    timerFlag1000ms = false;

    portEXIT_CRITICAL(&timerMux0);
}
