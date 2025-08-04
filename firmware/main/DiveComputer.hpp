#ifndef DIVE_COMPUTER_HPP
#define DIVE_COMPUTER_HPP

//The dive computer states,
#define STATE_DRY_IDLE 0 // dry idle is the computer in deep sleep mode above water 
#define STATE_DRY_ACTIVE 1 // dry active is the computer in active mode above water when the user is interacting with it
#define STATE_WET 2 // wet is the computer in active mode underwater, it is powered on and making readings fast

class DiveComputer {

public:
    unsigned char envState = STATE_DRY_IDLE; //The current state of the dive computer

    DiveComputer(); 
    void begin();
    void update(); 
}; 

#endif // DIVE_COMPUTER_HPP