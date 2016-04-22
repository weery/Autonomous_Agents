#include "Brain.h"

void Brain::InitializePins(byte pin_servo_wheel_left,byte pin_servo_wheel_right,
                            byte pin_servo_tower, byte pin_servo_claw,
                            byte pin_ultrasonic_lower_echo,byte pin_ultrasonic_lower_trig,
                            byte pin_ultrasonic_upper, byte pin_ir_reciever_left,
                            byte pin_ir_reciever_right,byte pin_ir_transmitter,
                            byte pin_phototransistor)
{
    // Assign all pins
    _pin_servo_wheel_left= pin_servo_wheel_left;
    _pin_servo_wheel_right= pin_servo_wheel_right,
    
    _pin_servo_tower= pin_servo_tower;
    _pin_servo_claw= pin_servo_claw;

    _pin_ultrasonic_lower_echo= pin_ultrasonic_lower_echo;
    _pin_ultrasonic_lower_trig= pin_ultrasonic_lower_trig;

    _pin_ultrasonic_upper= pin_ultrasonic_upper;
    
    _pin_ir_reciever_left= pin_ir_reciever_left;
    _pin_ir_reciever_right= pin_ir_reciever_right;
    
    _pin_ir_transmitter= pin_ir_transmitter;
    
    _pin_phototransistor= pin_phototransistor;

    // Assign pin input/output
    
    pinMode(_pin_ir_reciever_left,INPUT);
    pinMode(_pin_ir_reciever_right,INPUT);

    pinMode(_pin_ir_transmitter,OUTPUT);

    pinMode(_pin_ultrasonic_lower_trig,OUTPUT);
    pinMode(_pin_ultrasonic_lower_echo,INPUT);
    
    // Initialize servos
        
    _servo_wheel_left.attach(_pin_servo_wheel_left);
    _servo_wheel_right.attach(_pin_servo_wheel_right);

    _servo_tower.attach(_pin_servo_tower);
    _servo_claw.attach(_pin_servo_claw);
    
    
    _servo_signal_wheel_left = 1500;
    _servo_signal_wheel_right = 1500;

    _servo_signal_tower = 90;
    
    _servo_signal_claw = 90;
    
    _servo_wheel_left.writeMicroseconds(_servo_signal_wheel_left);
    _servo_wheel_right.writeMicroseconds(_servo_signal_wheel_right);

    _servo_tower.write(_servo_signal_tower);
    
    _servo_claw.write(_servo_signal_claw);
    
    
    // Intialize states
    _current_state = STATE_SEARCH;

    _current_movement = STATE_FORWARD;

    delay(100);
}

void Brain::Run()
{
    _update_counter=0;
    
    int ultrasonic_lower_reading = Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig); 
    int ultrasonic_upper_reading = Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

    byte phototransistor_reading = Brain::ReadPhototransistor(_pin_phototransistor);
    
    bool ir_left_reading = Brain::ReadIr(_pin_ir_reciever_left);
    bool ir_right_reading = Brain::ReadIr(_pin_ir_reciever_right);
    
    byte ir_left_distance_reading = Brain::ReadIrDistance(_pin_ir_reciever_left,_pin_ir_transmitter);
    byte ir_right_distance_reading = Brain::ReadIrDistance(_pin_ir_reciever_right,_pin_ir_transmitter);

    Serial.println(ir_right_distance_reading);
    
    switch(_current_state)
    {
        case STATE_FIND_SAFEZONE:
            break;
        case STATE_ROAM:
            break;
        case STATE_FIND_CAN:
            break;
    }
    /*
    switch(_current_movement)
    {
        case STATE_ROTATE_LEFT:
            _servo_signal_left = 1450;
            _servo_signal_right = 1450;
            _servo_signal_wanted_left = 1450;
            _servo_signal_wanted_right = 1450;
            break;
        case STATE_ROTATE_RIGHT:
            _servo_signal_left = 1550;
            _servo_signal_right = 1550;
            _servo_signal_wanted_left = 1550;
            _servo_signal_wanted_right = 1550;
            break;
        case STATE_FORWARD:
            _servo_signal_left = 1550;
            _servo_signal_right = 1450;
            _servo_signal_wanted_left = 1550;
            _servo_signal_wanted_right = 1450;
            break;
        case STATE_BACKWARD:
            _servo_signal_left = 1450;
            _servo_signal_right = 1550;
            _servo_signal_wanted_left = 1450;
            _servo_signal_wanted_right = 1550;
            break;
    }
    //ChangeServoSignal();
    _servo_left.writeMicroseconds(_servo_signal_left);
    _servo_right.writeMicroseconds(_servo_signal_right);
    */
    byte remaining_delay=UPDATE_DELAY-_update_counter;
    delay(remaining_delay);
}

void Brain::LogSensors(bool whisker_left, bool whisker_right, int ultrasonic_distance,
        byte ir_left, byte ir_right)
{
    Serial.print("Whisker Left: ");
    Serial.println(whisker_left);

    Serial.print(" Whisker Right: ");
    Serial.println(whisker_right);

    Serial.print(" Ultrasonic: ");
    Serial.println(ultrasonic_distance);

    Serial.print(" IR Left: ");
    Serial.println(ir_left);

    Serial.print(" IR Right: ");
    Serial.println(ir_right);
}

int Brain::ReadUltrasonic2Pin(byte pin_echo, byte pin_trig)
{
    short maximumRange = 400;
    short minimumRange = 0;

     digitalWrite(pin_trig, LOW);
     delayMicroseconds(2);

     digitalWrite(pin_trig, HIGH);
     delayMicroseconds(10);

     digitalWrite(pin_trig, LOW);
     float duration = pulseIn(pin_echo, HIGH);
     float distance = duration/58.2;
     if (distance >= maximumRange || distance <= minimumRange){
        distance = 1000;
     }
     
     return floor(distance);
}

int Brain::ReadUltrasonic1Pin(byte pin)
{
  pinMode(pin, OUTPUT);          // Set pin to OUTPUT
  digitalWrite(pin, LOW);        // Ensure pin is low
  delayMicroseconds(2);
  digitalWrite(pin,HIGH);       // Start ranging
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pin,LOW);        // End ranging
  pinMode(pin, INPUT);           // Set pin to INPUT
  float duration = pulseIn(pin, HIGH); // Read echo pulse
  float distance = duration / 74 / 2 *2.54;        // Convert to cm 
  return floor(distance);
}

bool Brain::ReadIrWithTransmitter(byte pin_reciever, byte pin_transmitter, unsigned short frequency)
{
    tone(pin_transmitter, frequency, 8);
    delay(1);
    bool ir = digitalRead(pin_reciever);
    return ir;
}

byte Brain::ReadIrDistance(byte pin_reciever, byte pin_transmitter)
{
    unsigned short frequencies[5] = {38000,39000,40000,41000,42000};
    byte distances[5] = {40,35,30,25,20};
    byte distance = 0;
    for(short i = 4; i >= 0; i--)
    {
        bool detection = Brain::ReadIrWithTransmitter(pin_reciever,pin_transmitter, frequencies[i]);
        if (!detection){
            distance = distances[i];
            break;
        }
    }
    return distance;
}

bool Brain::ReadIr(byte pin_reciever)
{
    bool ir = digitalRead(pin_reciever);
    return ir;
}


byte Brain::ReadPhototransistor(byte pin_phototransistor)
{
  return analogRead(pin_phototransistor);
}
