#include "Brain.h"

Brain::Brain()
{
}

Brain::~Brain(){
    
}

void Brain::InitializePins(byte pin_whiskers_left,byte pin_whiskers_right, byte 
pin_servo_left,byte pin_servo_right ){
    _pin_whiskers_right=pin_whiskers_right;
    _pin_whiskers_left=pin_whiskers_left;
    pinMode(_pin_whiskers_left,INPUT);
    pinMode(_pin_whiskers_right,INPUT);
    
    _pin_servo_right=pin_servo_right;
    _servo_right.attach(_pin_servo_right);
    _servo_right.writeMicroseconds(1300);
    
    _pin_servo_left=pin_servo_left;
    _servo_left.attach(_pin_servo_left);
    _servo_left.writeMicroseconds(1700);
    
    _current_state= STATE_FORWARD;
}

void Brain::Run()
{
    byte wLeft = digitalRead(_pin_whiskers_left);
    byte wRight = digitalRead(_pin_whiskers_right);
    _timer_tracking++;
    
    switch(_current_state)
    {
        case STATE_FORWARD:
            _servo_right.writeMicroseconds(1300);
            _servo_left.writeMicroseconds(1700);
            if (wLeft == 0 || wRight == 0)
            {
                _current_state=STATE_BACKWARD;
                _timer_tracking=0;
            }
        break;
        case STATE_BACKWARD:
            _servo_right.writeMicroseconds(1700);
            _servo_left.writeMicroseconds(1300);
            if (_timer_tracking == _BACKING_ITERATIONS)
            {
                byte r = rand() % 2;
                Serial.println(r);
                
                if (r==0)
                    _current_state=STATE_TURN_LEFT;
                else
                    _current_state=STATE_TURN_RIGHT;
                
                _timer_tracking=0;
            }
        break;    
        case STATE_TURN_LEFT:
            _servo_right.writeMicroseconds(1300);
            _servo_left.writeMicroseconds(1300);
            if (_timer_tracking == _TURNING_ITERATIONS)
            {
                _current_state=STATE_FORWARD;
            }
        break;    
        case STATE_TURN_RIGHT:
            _servo_right.writeMicroseconds(1700);
            _servo_left.writeMicroseconds(1700);
            if (_timer_tracking == _TURNING_ITERATIONS)
            {
                _current_state=STATE_FORWARD;
            }
        break; 
    }
    
            
        
    
    delay(50);
}