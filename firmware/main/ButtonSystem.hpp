#ifndef BUTTON_SYSTEM_HPP
#define BUTTON_SYSTEM_HPP

#define BUT0_APIN A6
#define BUT1_APIN A7

#define BUT_DEBOUNCE_DELAY 50 // milliseconds to debounce presses for
#define BUT_THRESHOLD 900 // Analog reading threshold for button press

class ButtonSystem {

private:
    bool but0LastReading; 
    bool but0State; 
    unsigned long but0Time;

    bool but1LastReading; 
    bool but1State; 
    unsigned long but1Time;

public:
    ButtonSystem(); 
    void begin();
    void update(); 
}; 

#endif // BUTTON_SYSTEM_HPP