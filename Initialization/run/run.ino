#include <Servo.h>

enum State {
  Drive_Forward,
  Drive_Backward,
  Stop,
  Turn_Right,
  Turn_Left
};

State currentState = Drive_Forward;


Servo servoLeft;
Servo servoRight;

int servoLeftPin;
int servoRightPin;

int maxPulseWidth = 1700;
int minPulseWidth = 1300;
int middlePulseWidth = 1500;

int lightSensorPin;

float READBLACK = 0.17;

void setup() {
  lightSensorPin = 3;

  servoLeftPin = 13;
  servoLeft.attach(servoLeftPin);
  servoLeft.writeMicroseconds(middlePulseWidth);

  servoRightPin = 12;
  servoRight.attach(servoRightPin);
  servoRight.writeMicroseconds(middlePulseWidth);
  Serial.begin(9600);
}

void loop() {
  float volt = volts(lightSensorPin);
  Serial.println(volt);

  switch (currentState)
  {
    case Drive_Forward:
      servoLeft.writeMicroseconds(maxPulseWidth);
      servoRight.writeMicroseconds(minPulseWidth);
      if (volt < READBLACK)
      {
        currentState = Stop;
      }
      break;
    case Drive_Backward:
      servoLeft.writeMicroseconds(minPulseWidth);
      servoRight.writeMicroseconds(maxPulseWidth);
      break;
    case Stop:
      servoLeft.writeMicroseconds(middlePulseWidth);
      servoRight.writeMicroseconds(middlePulseWidth);
      break;
    case Turn_Left:
      servoLeft.writeMicroseconds(minPulseWidth);
      servoRight.writeMicroseconds(minPulseWidth);
      break;
    case Turn_Right:
      servoLeft.writeMicroseconds(maxPulseWidth);
      servoRight.writeMicroseconds(maxPulseWidth);
      break;
  }
}


float volts(int adPin)
{
  return float(analogRead(adPin)) * 5.0 / 1024.0;
}


