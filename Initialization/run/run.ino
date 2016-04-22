
void setup() {
  lightSensorPin = 3;
}

void loop() {
  Serial.println(analogRead(lightSensorPin));
}


float volts(int adPin)
{
  return float(analogRead(adPin)) * 5.0 / 1024.0;
}


