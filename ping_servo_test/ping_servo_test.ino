#include "Brain.h"

Brain brain;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  brain.InitializePins(11,10);
  // Ping Servo = 11
  // Ultrasonic Sensor = 10;
}

void loop() {
  // put your main code here, to run repeatedly:
  brain.Run();
}
