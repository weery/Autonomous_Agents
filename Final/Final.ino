#include "Brain.h"

Brain brain;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  brain.InitializePins(12,13,11,0,6,8,10,9,3,4,1);
  // Wheel left = 13 ? 
  // Wheel right = 12 ?
  // tower = 11 
  // claw = 
  // ultrasonic echo = 6
  // ultrasonic trig = 8
  // ultrasonic upper = 10
  // ir reciever left = 9
  // ir reciever right = 3
  // ir transmitter = 4
  // phototransistor = 1 (analog)
}

void loop() {
  // put your main code here, to run repeatedly:
  brain.Run();
}
