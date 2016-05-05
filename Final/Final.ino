#include "Brain.h"

Brain brain;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  brain.InitializePins(12,13,11,2,6,8,10,9,3,5,7,4,1);
  // Digital Pins (0,1,2,3,4,5,6,7,8,9,10,11,12,13)
  // Taken   Pins (-,-,x,x,x,x,x,x,x,x,xx,xx,xx,xx)
  // Analog Pins  (0,1,2,3,4,5)
  // Taken  Ping  (-,x,0,0,0,0)
  // Wheel left = 12
  // Wheel right = 13
  // tower = 11 
  // claw = 2
  // ultrasonic echo = 6
  // ultrasonic trig = 8
  // ultrasonic upper = 10
  // ir reciever left_front = 9
  // ir reciever right_front = 3
  // ir reciever left_back = 5?
  // ir reciever right_back = 7?
  // ir transmitter = 4
  // phototransistor = 1 (analog)
}

void loop() {
  // put your main code here, to run repeatedly:
  brain.Run();
}
