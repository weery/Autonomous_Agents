#ifndef BRAIN_H
#define BRAIN_H


#include <Arduino.h>
#include <Servo.h>

class Brain
{
    public:
        void InitializePins(byte pin_whiskers_left,byte pin_whiskers_right, byte pin_servo_left, byte pin_servo_right, byte pin_ir_reciever_left, byte pin_ir_reciever_right, byte pin_ir_transmitter_left,byte pin_ir_transmitter_right, byte pin_ultrasonic_trig, byte pin_ultrasonic_echo, byte pin_speaker);
        void Run();
        
    private:
        byte _pin_whiskers_left;
        byte _pin_whiskers_right;
        
        byte _pin_servo_left;
        byte _pin_servo_right;
        
        
        byte _pin_ir_reciever_left;
        byte _pin_ir_reciever_right;
        
        byte _pin_ir_transmitter_left;
        byte _pin_ir_transmitter_right;
        
        byte _pin_ultrasonic_trig;
        byte _pin_ultrasonic_echo;
        
        byte _pin_speaker;
        
        Servo _servo_left;
        Servo _servo_right;
        
        unsigned short _servo_signal_wanted_left;
        unsigned short _servo_signal_wanted_right;
        
        unsigned short _servo_signal_left;
        unsigned short _servo_signal_right;
        
        enum BrainStates{
            STATE_SEARCH = 0,
            STATE_LOCALIZE = 1,
            STATE_GOTO = 2,
        };
        
        enum MovementStates{
            STATE_ROTATE_LEFT = 0,
            STATE_ROTATE_RIGHT = 1,
            STATE_FORWARD = 2,
            STATE_BACKWARD = 3,
        };
        
        BrainStates _current_state;
        
        MovementStates _current_movement;
        
        byte _speed;
        const char MAX_SPEED = 125;
        const char MIN_SPEED = -125;
        
        const short UPPER_LIMIT = 1700;
        const short LOWER_LIMIT = 1300;
        
        int ReadUltrasonic();
        
        bool ReadWhiskers(byte pin_nr);
        
        byte ReadIR(byte pin_reciever, byte pin_transmitter);
        
        bool irDetect(byte irLedPin, byte irReceiverPin, unsigned short frequency);
        
        void ChangeServoSignal();
};



#endif