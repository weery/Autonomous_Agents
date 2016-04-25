/*
  Script used to calibrate the wheel servos.
*/

#include <Servo.h>

Servo servo;

const byte servo_pin = 12;
int calibration_speed = 1500;

void setup() {
 servo.attach(servo_pin);
}

void loop() {
 servo.write(calibration_speed);
 delay(20);
}
