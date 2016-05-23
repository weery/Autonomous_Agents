/*
  Script used to calibrate the wheel servos.
*/

#include <Servo.h>

Servo servo1;
Servo servo2;

const byte servo_pin1 = 12;
const byte servo_pin2 = 13;
int calibration_speed = 1500;

void setup() {
 servo1.attach(servo_pin1);
 servo2.attach(servo_pin2);
}

void loop() {
 servo1.write(calibration_speed);
 servo2.write(calibration_speed);
 delay(20);
}
