#ifndef BRAIN_H
#define BRAIN_H


#include <Arduino.h>
#include <Servo.h>

class Brain
{
    public:
        void InitializePins(byte pin_servo_ping, byte pin_sensor_ultrasonic_trig, byte pin_sensor_ultrasonic_echo, byte pin_sensor_ultrasonic_upper);
        void Run();

    private:
    
        byte _pin_servo_ping;
        byte _pin_sensor_ultrasonic_trig;
        byte _pin_sensor_ultrasonic_echo;
        byte _pin_sensor_ultrasonic_upper;
        
        Servo _servo_ping;
        
        unsigned short _servo_ping_signal;
        
        int RCTime(byte sensorIn);
        
        int ReadUltrasonic();
        
        int ReadUltrasonic4Pin();
};



#endif
