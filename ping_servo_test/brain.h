#ifndef BRAIN_H
#define BRAIN_H


#include <Arduino.h>
#include <Servo.h>

class Brain
{
    public:
        void InitializePins(byte pin_servo_ping, byte pin_sensor_ultrasonic);
        void Run();

    private:
    
        byte _pin_servo_ping;
        byte _pin_sensor_ultrasonic;
        
        Servo _servo_ping;
        
        unsigned short _servo_ping_signal;
        
        long RCTime(byte sensorIn);
};



#endif
