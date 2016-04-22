int lightSensorPin;

void setup() {
  lightSensorPin = 1;
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(lightSensorPin));
}
