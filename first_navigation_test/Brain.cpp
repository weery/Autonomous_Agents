#include "Brain.h"

void Brain::InitializePins(byte pin_whiskers_left, byte pin_whiskers_right, byte pin_servo_left, byte pin_servo_right, byte pin_ir_reciever_left, byte pin_ir_reciever_right, byte pin_ir_transmitter_left,byte pin_ir_transmitter_right, byte pin_ultrasonic_trig, byte pin_ultrasonic_echo, byte pin_speaker)
{
    /*
    _pin_whiskers_left = pin_whiskers_left;
    _pin_whiskers_right = pin_whiskers_right;
    
    _pin_servo_left = pin_servo_left;
    _pin_servo_right = pin_servo_right;*/
    
    _pin_ir_reciever_left = pin_ir_reciever_left;
    _pin_ir_reciever_right = pin_ir_reciever_right;
    
    _pin_ir_transmitter_left = pin_ir_transmitter_left;
    _pin_ir_transmitter_right = pin_ir_transmitter_right;
    
    /*
    _pin_ultrasonic_trig = pin_ultrasonic_trig;
    _pin_ultrasonic_echo = pin_ultrasonic_echo;*/
    
    _pin_speaker = pin_speaker;
    /*
    pinMode(_pin_whiskers_left,INPUT);
    pinMode(_pin_whiskers_right,INPUT);
    
    _servo_left.attach(_pin_servo_left);
    _servo_right.attach(_pin_servo_right);*/
    
    pinMode(_pin_ir_reciever_left,INPUT);
    pinMode(_pin_ir_reciever_right,INPUT);
    
    pinMode(_pin_ir_transmitter_left,OUTPUT);
    pinMode(_pin_ir_transmitter_right,OUTPUT);
    
    /*
    pinMode(_pin_ultrasonic_trig,OUTPUT);
    pinMode(_pin_ultrasonic_echo,OUTPUT);*/
    
    _current_state = STATE_SEARCH;
    
    _current_movement = STATE_FORWARD;
    
    _speed = MAX_SPEED;
    
    tone(_pin_speaker,2000,100);
    
    delay(100);
}

void Brain::Run()
{
    //bool whisker_left =  Brain::ReadWhiskers(_pin_whiskers_left);
    //bool whisker_right = Brain::ReadWhiskers(_pin_whiskers_right);
    
    //int ultrasonic_distance = Brain::ReadUltrasonic();
    
    byte ir_left = Brain::ReadIR(_pin_ir_reciever_left,_pin_ir_transmitter_left);
    byte ir_right = Brain::ReadIR(_pin_ir_reciever_right,_pin_ir_transmitter_right);
    
    /*
    Serial.print("Whisker Left: ");
    Serial.print(whisker_left);
    
    Serial.print(" Whisker Right: ");
    Serial.print(whisker_right);
    
    Serial.print(" Ultrasonic: ");
    Serial.print(ultrasonic_distance);
    
    Serial.print(" IR Left: ");
    Serial.print(ir_left);
    
    Serial.print(" IR Right: ");
    Serial.println(ir_right);*/
    
    switch(_current_state)
    {
        case STATE_SEARCH:
            break;
        case STATE_LOCALIZE:
            break;
        case STATE_GOTO:
            break;
    }
    
    switch(_current_movement)
    {
        case STATE_ROTATE_LEFT:
            break;
        case STATE_ROTATE_RIGHT:
            break;
        case STATE_FORWARD:
            break;
        case STATE_BACKWARD:
            break;
    }
    
    delay(100);
}

void Brain::ChangeServoSignal()
{
    _servo_signal_left += copysign(1,_servo_signal_wanted_left-_servo_signal_left)*50;
    _servo_signal_Right += copysign(1,_servo_signal_wanted_right-_servo_signal_Right)*50;
}

int Brain::ReadUltrasonic()
{
    short maximumRange = 400;
    short minimumRange = 0;
    
     digitalWrite(_pin_ultrasonic_trig, LOW); 
     delayMicroseconds(2); 

     digitalWrite(_pin_ultrasonic_trig, HIGH);
     delayMicroseconds(10); 
     
     digitalWrite(_pin_ultrasonic_trig, LOW);
     short duration = pulseIn(_pin_ultrasonic_echo, HIGH);
     
     short distance = duration/58.2;
     
     if (distance >= maximumRange || distance <= minimumRange){
        distance = -1;
     }
     
     delay(50);
     
     return distance;
}

bool Brain::ReadWhiskers(byte pin_nr)
{
    bool whiskers_state = digitalRead(pin_nr);
    return whiskers_state;
}


byte Brain::ReadIR(byte pin_reciever, byte pin_transmitter)
{
    delay(100);
    unsigned short frequencies[5] = {38000,39000,40000,41000,42000};
    byte distances[5] = {40,35,30,25,20};
    byte distance = 0;
    for(short i = 4; i >= 0; i--)
    {
        bool detection = Brain::irDetect(pin_transmitter, pin_reciever, frequencies[i]);
        if (!detection){
            distance = distances[i];
            break;
        }
    }
    return distance;
}

bool Brain::irDetect(byte irLedPin, byte irReceiverPin, unsigned short frequency)
{
  tone(irLedPin, frequency, 8);              
  delay(1);                                  
  int ir = digitalRead(irReceiverPin);       
  delay(10);                                  
  return ir;                                 
}     