#include "Brain.h"

void Brain::InitializePins(byte pin_servo_ping, byte pin_sensor_ultrasonic_trig,byte pin_sensor_ultrasonic_echo, byte pin_sensor_ultrasonic_upper)
{
    _pin_servo_ping = pin_servo_ping;
    _servo_ping.attach(_pin_servo_ping);
    
    _pin_sensor_ultrasonic_trig=pin_sensor_ultrasonic_trig;
    _pin_sensor_ultrasonic_echo=pin_sensor_ultrasonic_echo;
    
    _pin_sensor_ultrasonic_upper=pin_sensor_ultrasonic_upper;
    
    pinMode(_pin_sensor_ultrasonic_echo,INPUT);
    pinMode(_pin_sensor_ultrasonic_trig,OUTPUT);
    
    _servo_ping_signal=90;
}

void Brain::Run()
{
    int readings_lower[38] = {};
    int readings_upper[38] = {};
    int min_reading = 1000;
    int min_angle = 90;
    /*
    for (int i = 0; i <= 18; i++)
    {
        int angle = 45+i*5;
        _servo_ping.write(angle);
        int ultra_upper= Brain::RCTime(_pin_sensor_ultrasonic_upper);
        int ultra_lower=Brain::ReadUltrasonic4Pin();
        readings_lower[i]=ultra_lower;
        readings_upper[i]=ultra_upper;
        Serial.print("diff");
        Serial.println(ultra_upper-ultra_lower);
        if (ultra_lower<min_reading && ultra_upper-ultra_lower>1)
        {
            min_reading = ultra_lower;
            min_angle = angle;
        }
        delay(20);
    }
    for (int i = 19; i <=37 ; i++)
    {
        int angle = 135-(i-19)*5;
        _servo_ping.write(angle);
        int ultra_upper= Brain::RCTime(_pin_sensor_ultrasonic_upper);
        int ultra_lower=Brain::ReadUltrasonic4Pin();
        readings_lower[i]=ultra_lower;
        readings_upper[i]=ultra_upper;
        Serial.print("diff");
        Serial.println(ultra_upper-ultra_lower);
        if (ultra_lower<min_reading && ultra_upper-ultra_lower>1)
        {
            min_reading = ultra_lower;
            min_angle = angle;
        }
        delay(20);
    }
    
    Serial.print("Best Length:");
    Serial.println(min_reading);
    
    */
    Serial.print("Best angle:");
    Serial.println(min_angle);
    
    
    _servo_ping.write(min_angle);
    
    delay(2000);
}


int Brain::ReadUltrasonic()
{
    int ultra_upper= Brain::RCTime(_pin_sensor_ultrasonic_upper);
}

int Brain::ReadUltrasonic4Pin()
{
    short maximumRange = 400;
    short minimumRange = 0;

     digitalWrite(_pin_sensor_ultrasonic_trig, LOW);
     delayMicroseconds(2);

     digitalWrite(_pin_sensor_ultrasonic_trig, HIGH);
     delayMicroseconds(10);

     digitalWrite(_pin_sensor_ultrasonic_trig,LOW);
     float duration = pulseIn(_pin_sensor_ultrasonic_echo, HIGH);
     float distance = duration/58.2;
     Serial.print("Ultrasonic Lower:");
     Serial.println(floor(distance));
     if (distance >= maximumRange || distance <= minimumRange){
        distance = -1;
     }


     return floor(distance);
}
    
int Brain::RCTime(byte pingPin){
  pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
  digitalWrite(pingPin, LOW);        // Ensure pin is low
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);       // Start ranging
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pingPin, LOW);        // End ranging
  pinMode(pingPin, INPUT);           // Set pin to INPUT
  float duration = pulseIn(pingPin, HIGH); // Read echo pulse
  float distance = duration / 74 / 2 *2.54;        // Convert to cm 
  delay(200);
  return floor(distance);
}