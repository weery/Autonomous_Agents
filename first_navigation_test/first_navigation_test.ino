#include "Brain.h"

Brain brain;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  brain.InitializePins(0,0,0,0,3,9,2,8,0,0,4);
  // Whisker Left =
  // Whisker Right =
  // Servo Left =
  // Servo Right =
  // IR Reciever Left = 3
  // IR Reciever Right = 2
  // IR Transmitter Left = 9 
  // IR Transmitter Right = 8
  // Ultrasonic Trig = 
  // Ultrasonic Echo = 
  // Speaker = 4
}

void loop() {
  // put your main code here, to run repeatedly:
  brain.Run();
}
