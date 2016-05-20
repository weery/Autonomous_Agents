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
                            byte pin_ultrasonic_upper, byte pin_ir_reciever_left_front,
                            byte pin_ir_reciever_right_front,byte pin_ir_reciever_left_back,
                            byte pin_ir_reciever_right_back, byte pin_ir_transmitter_left_front,
                            byte pin_ir_transmitter_right_front, byte pin_ir_transmitter_left_back,
                            byte pin_ir_transmitter_richt_back, byte pin_phototransistor, byte pin_whiskers,
                            byte pin_led);
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

        byte _pin_ir_reciever_left_front;
        byte _pin_ir_reciever_right_front;

        byte _pin_ir_reciever_left_back;
        byte _pin_ir_reciever_right_back;

        byte _pin_ir_transmitter_left_front;
        byte _pin_ir_transmitter_right_front;

        byte _pin_ir_transmitter_left_back;
        byte _pin_ir_transmitter_right_back;

        byte _pin_phototransistor;

        byte _pin_whiskers;
        
        byte _pin_led;

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
            STATE_ROTATE_LEFT_SLOWLY = 9,
            STATE_ROTATE_RIGHT_SLOWLY = 10,
            STATE_FORWARD = 3,
            STATE_BACKWARD = 4,
            STATE_BACKWARD_RIGHT =5,
            STATE_BACKWARD_LEFT =6,
            STATE_FORWARD_RIGHT =7,
            STATE_FORWARD_LEFT =8,
        };

        enum MovementActions{
            ACTION_LOCKED = 0,
            ACTION_UNDECIDED = 1,
        };

        enum Behaviours{
            ROAM = 0, // Same as default
            LOCALIZE_BEACON = 1, // Face beacon, else roam
            HEAD_TO_BEACON = 2,
            GO_TO_BEACON = 3, // Go Towards the found beacon
            LEAVE_CAN = 4, // When at safezone/beacon, release can and back
            LOCALIZE_CAN = 5, // FInd can + reading
            HEAD_TO_CAN = 6,
            GO_TO_CAN = 7, // Go Towards found can
            CATCH_CAN = 8, // When at can, lower arm
            TEST_SENSOR = 9,
            LEAVE_SAFE_ZONE = 10,
        };

        BrainStates _current_state;

        MovementStates _current_movement;

        MovementActions _movement_action;

        Behaviours _current_behaviour;

        // Behaviours
        void LocalizeBeacon();
        void HeadToBeacon();
        void GoToBeacon();
        void LeaveCan();
        void LocalizeCan();
        void HeadToCan();
        void GoToCan();
        void CatchCan();
        void Roam();
        void LeaveSafeZone();
        bool CheckHasCan();
        bool IsAtBeacon();

        // Obstacle avoidance
        bool AvoidCollision(byte distance);

        // Read sensors
        int ReadUltrasonic2Pin(byte pin_echo, byte pin_trig);

        int ReadUltrasonic1Pin(byte pin);

        byte ReadIrDistance(byte pin_reciever, byte pin_transmitter);

        bool ReadIr(byte pin_reciever);

        bool ReadIrWithTransmitter(byte pin_reciever, byte pin_transmitter, unsigned short frequency);

        float ReadPhototransistor(byte pin_phototransistor);

        unsigned short Clamp(unsigned short val, unsigned short max, unsigned short min);

        // other
        void LogSensors(bool whisker_left, bool whisker_right, int ultrasonic_distance,
        byte ir_left, byte ir_right);

        void GoToLocalizeCan();

        byte _update_counter;

        const byte UPDATE_DELAY = 100;
        const byte BLACK_PAPER_LIMIT = 10;

        const byte COLLISION_DISTANCE_SHORT = 10;
        const byte COLLISION_DISTANCE_MIDDLE = 15;
        const byte COLLISION_DISTANCE_SEMILONG = 21;
        const byte COLLISION_DISTANCE_LONG = 26;

        byte _can_reading = 255;
        byte _can_angle;

        byte movement_time = 0;

        bool has_can = false;

        // servo constants
        const byte MAX_ANGLE = 135;
        const byte MIDDLE_ANGLE = 90;
        const byte MIN_ANGLE = 45;
        const unsigned short MAX_SIGNAL = 1600;
        const unsigned short MIDDLE_SIGNAL = 1500;
        const unsigned short MIN_SIGNAL = 1400;

        // Find can constants
        const byte ULTRASONIC_DIFF_MARGIN = 6;

        const byte MIN_PINGSENSOR_READING = 13;

        // Avoid Constant
        short CollisionTimer = 0;

        // Max move constants
        const byte MAX_LOCALIZING_BEACON = 50;
        const byte MAX_MOVEMENT_TOWARDS_CAN = 10;

        void ChangeWheelServos();
        void ChangeClawServo();
        void ChangeTowerServo();
};

#endif

