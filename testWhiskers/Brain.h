#ifndef Brain_h
#define Brain_h

#include "Arduino.h"
#include "Servo.h"

class Brain { 
public:
	Brain();
	~Brain();
    void InitializePins(byte pin_whiskers_left,byte pin_whiskers_right, byte 
pin_servo_left,byte pin_servo_right );
    void Run();
private:
    byte _pin_whiskers_left;
    byte _pin_whiskers_right;
    
    byte _pin_servo_left;
    byte _pin_servo_right;
    
    Servo _servo_right;
    Servo _servo_left;
    
    enum EnumStates{
      STATE_FORWARD = 0,
      STATE_BACKWARD = 1,
      STATE_TURN_LEFT = 2,
      STATE_TURN_RIGHT = 3
    };
    
    EnumStates _current_state;
    
    const byte _BACKING_ITERATIONS = 20;
    byte _timer_tracking;
    
    const byte _TURNING_ITERATIONS = 10;
};

#endif