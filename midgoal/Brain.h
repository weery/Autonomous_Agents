#ifndef BRAIN_H
#define BRAIN_H


#include <Arduino.h>
#include <Servo.h>
#include <stdlib.h>

class Brain
{
    public:
        void InitializePins(byte pin_servo_wheel_left,byte pin_servo_wheel_right,
                            byte pin_servo_tower, byte pin_servo_claw,
                            byte pin_ultrasonic_lower_echo,byte pin_ultrasonic_lower_trig,
                            byte pin_ultrasonic_upper, byte pin_ir_reciever_left,
                            byte pin_ir_reciever_right,byte pin_ir_transmitter,
                            byte pin_phototransistor);
        void Run();

    private:
    // Pin locations
        byte _pin_servo_wheel_left;
        byte _pin_servo_wheel_right;

        byte _pin_servo_tower;

        byte _pin_servo_claw;

        byte _pin_ultrasonic_lower_echo;
        byte _pin_ultrasonic_lower_trig;

        byte _pin_ultrasonic_upper;

        byte _pin_ir_reciever_left;
        byte _pin_ir_reciever_right;

        byte _pin_ir_transmitter;

        byte _pin_phototransistor;

    // Servos
        Servo _servo_wheel_left;
        Servo _servo_wheel_right;

        Servo _servo_tower;

        Servo _servo_claw;

    // Servo Signals

        unsigned short _servo_signal_wheel_left; // 1300-1700, >1500 forward
        unsigned short _servo_signal_wheel_right; // 1300-1700, <1500 backward

        unsigned short _servo_signal_tower; // 0-180, 90 middle

        unsigned short _servo_signal_claw; // 0-180, 90 middle

        enum BrainStates{
            STATE_FIND_SAFEZONE = 0,
            STATE_ROAM = 1,
            STATE_FIND_CAN = 2,
            STATE_TEST_SENSOR = 3,
        };

        enum MovementStates{
            STATE_STOP = 0,
            STATE_ROTATE_LEFT = 1,
            STATE_ROTATE_RIGHT = 2,
            STATE_FORWARD = 3,
            STATE_BACKWARD = 4,
            STATE_BACKWARD_RIGHT =5,
            STATE_BACKWARD_LEFT =6,
        };

        enum MovementActions{
            ACTION_LOCKED = 0,
            ACTION_UNDECIDED = 1,
        };

        BrainStates _current_state;

        MovementStates _current_movement;

        MovementActions _movement_action;

        // Read sensors
        int ReadUltrasonic2Pin(byte pin_echo, byte pin_trig);

        int ReadUltrasonic1Pin(byte pin);

        byte ReadIrDistance(byte pin_reciever, byte pin_transmitter);

        bool ReadIr(byte pin_reciever);

        bool ReadIrWithTransmitter(byte pin_reciever, byte pin_transmitter, unsigned short frequency);

        byte ReadPhototransistor(byte pin_phototransistor);


        // other
        void LogSensors(bool whisker_left, bool whisker_right, int ultrasonic_distance,
        byte ir_left, byte ir_right);

        const byte UPDATE_DELAY = 100;
        const byte BLACK_PAPER_LIMIT = 10;
        byte _update_counter;
        byte movement_time = 0;
};



#endif
