#ifndef MCU_SYSTEM_HPP
#define MCU_SYSTEM_HPP

#include <SPI.h>
#include "driver/spi_common.h" // Needed for SPI2_HOST, SPI3_HOST
#include <Wire.h> //I2C library
#include <Arduino.h>

#define _TIMERINTERRUPT_LOGLEVEL_     4               //From 0 - 4 No idea what this means
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default

#include <ESP32Time.h>

#define SPI_MISO 12
#define SPI_MOSI 11
#define SPI_CLK 13

#define TIMER0_PERIOD_MS 100 

class MCUSystem {
private:
    hw_timer_t *timer0 = NULL;
    //ESP32Time rtc(offset);

    static void IRAM_ATTR timer0ISR();
    static portMUX_TYPE timerMux0;

public:
    volatile static uint16_t timer0Counter; 

    //Flags can be read and cleared in the main loop
    volatile static bool timerFlag100ms;
    volatile static bool timerFlag500ms;
    volatile static bool timerFlag1000ms;

    SPIClass spiClass;

    MCUSystem(); 
    void begin();
    void initHardwareTimers(); 
    void resetTimers(); 
}; 

#endif // MCU_SYSTEM_HPP