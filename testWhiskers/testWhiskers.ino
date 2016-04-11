#include <Brain.h>

const byte WHISKER_RIGHT = 5;
const byte WHISKER_LEFT = 7;

const byte SERVO_RIGHT = 12;
const byte SERVO_LEFT = 13;

Brain brain;

void setup() {
  // put your setup code here, to run once:
  brain.InitializePins(WHISKER_LEFT,WHISKER_RIGHT,SERVO_LEFT,SERVO_RIGHT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    brain.Run();
}
