#include "Brain.h"

Brain brain;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  brain.InitializePins(4,6,13,12,9,3,0,2,8,6,5);
  // Whisker Left = 4
  // Whisker Right = 6
  // Servo Left = 13
  // Servo Right = 12
  // IR Reciever Left = 9
  // IR Reciever Right = 3
  // IR Transmitter Left = 8 
  // IR Transmitter Right = 2
  // Ultrasonic Trig = 11
  // Ultrasonic Echo = 10
  // Speaker = 5
}

void loop() {
  // put your main code here, to run repeatedly:
  brain.Run();
}
