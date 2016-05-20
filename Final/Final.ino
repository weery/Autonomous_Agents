#include "Brain.h"

Brain brain;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  brain.InitializePins(13,12,11,9,7,8,10,6,3,5,4,19,16,18,17,14,15,2);
  // Digital Pins (0,1,2,3,4,5,6,7,8,9,10,11,12,13)
  // Taken   Pins (-,-,x,x,x,x,x,x,x,x,xx,xx,xx,xx)
  // Analog Pins  (0,1,2,3,4,5)
  // Taken  Ping  (-,x,0,0,0,0)
  // Wheel left = 12
  // Wheel right = 13
  // tower = 11
  // claw = 9
  // ultrasonic echo = 7
  // ultrasonic trig = 8
  // ultrasonic upper = 10
  // ir reciever left_front = 6
  // ir reciever right_front = 3
  // ir reciever left_back = 5
  // ir reciever right_back = 4
  // ir transmitter left front = 19  (analog)
  // ir transmitter right front = 16 (analog)
  // ir transmitter left back =  18 (analog)
  // ir transmitter right back =  17 (analog)
  // phototransistor = 14
  // Whiskers = 15
  // LED = 2
}

void loop() {
  // put your main code here, to run repeatedly:
  brain.Run();
}
