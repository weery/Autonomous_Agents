#include "Brain.h"

void Brain::InitializePins(byte pin_servo_ping, byte pin_sensor_ultrasonic)
{
    _pin_servo_ping = pin_servo_ping;
    _servo_ping.attach(_pin_servo_ping);
    _pin_sensor_ultrasonic=pin_sensor_ultrasonic;
    
    _servo_ping_signal=90;
}

void Brain::Run()
{
    _servo_ping.write(105); // 135 MAX
    Brain::RCTime(_pin_sensor_ultrasonic);
    delay(20);
}



    
long Brain::RCTime(byte pingPin){
  pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
  digitalWrite(pingPin, LOW);        // Ensure pin is low
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);       // Start ranging
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pingPin, LOW);        // End ranging
  pinMode(pingPin, INPUT);           // Set pin to INPUT
  float duration = pulseIn(pingPin, HIGH); // Read echo pulse
  float inches = duration / 74 / 2 *2.54;        // Convert to inches
  Serial.println(inches);          // Short delay
  return inches;
}