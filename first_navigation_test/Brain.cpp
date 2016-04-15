#include "Brain.h"

void Brain::InitializePins(byte pin_whiskers_left, byte pin_whiskers_right, byte pin_servo_left, byte pin_servo_right, byte pin_ir_reciever_left, byte pin_ir_reciever_right, byte pin_ir_transmitter_left,byte pin_ir_transmitter_right, byte pin_ultrasonic_trig, byte pin_ultrasonic_echo, byte pin_speaker)
{
    _pin_whiskers_left = pin_whiskers_left;
    _pin_whiskers_right = pin_whiskers_right;


    _pin_servo_left = pin_servo_left;
    _pin_servo_right = pin_servo_right;

    _pin_ir_reciever_left = pin_ir_reciever_left;
    _pin_ir_reciever_right = pin_ir_reciever_right;

    _pin_ir_transmitter_left = pin_ir_transmitter_left;
    _pin_ir_transmitter_right = pin_ir_transmitter_right;


    _pin_ultrasonic_trig = pin_ultrasonic_trig;
    _pin_ultrasonic_echo = pin_ultrasonic_echo;

    _pin_speaker = pin_speaker;

    pinMode(_pin_whiskers_left,INPUT);
    pinMode(_pin_whiskers_right,INPUT);


    _servo_left.attach(_pin_servo_left);
    _servo_right.attach(_pin_servo_right);

    _servo_left.writeMicroseconds(1500);
    _servo_right.writeMicroseconds(1500);

    pinMode(_pin_ir_reciever_left,INPUT);
    pinMode(_pin_ir_reciever_right,INPUT);

    pinMode(_pin_ir_transmitter_left,OUTPUT);
    pinMode(_pin_ir_transmitter_right,OUTPUT);


    pinMode(_pin_ultrasonic_trig,OUTPUT);
    pinMode(_pin_ultrasonic_echo,INPUT);

    _current_state = STATE_SEARCH;

    _current_movement = STATE_FORWARD;

    _speed = MAX_SPEED;

    tone(_pin_speaker,2000,100);

    delay(100);
}

void Brain::Run()
{
    bool whisker_left =  Brain::ReadWhiskers(_pin_whiskers_left);
    bool whisker_right = Brain::ReadWhiskers(_pin_whiskers_right);

    int ultrasonic_distance = Brain::ReadUltrasonic();

    byte ir_left = Brain::ReadIR(_pin_ir_reciever_left,_pin_ir_transmitter_left);
    byte ir_right = Brain::ReadIR(_pin_ir_reciever_right,_pin_ir_transmitter_right);
    /*
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
*/
    switch(_current_state)
    {
        case STATE_SEARCH:
            if (!whisker_left){
                _current_movement = STATE_BACKWARD;
            }

            break;
        case STATE_LOCALIZE:
            break;
        case STATE_GOTO:
            break;
    }

    switch(_current_movement)
    {
        case STATE_ROTATE_LEFT:
            _servo_signal_wanted_left = 1400;
            _servo_signal_wanted_right = 1400;
            break;
        case STATE_ROTATE_RIGHT:
            _servo_signal_wanted_left = 1600;
            _servo_signal_wanted_right = 1600;
            break;
        case STATE_FORWARD:
            _servo_signal_wanted_left = 1600;
            _servo_signal_wanted_right = 1400;
            break;
        case STATE_BACKWARD:
            _servo_signal_wanted_left = 1400;
            _servo_signal_wanted_right = 1600;
            break;
    }
    ChangeServoSignal();
    _servo_left.writeMicroseconds(_servo_signal_left);
    _servo_right.writeMicroseconds(_servo_signal_right);
    delay(20);
}

// change signal based on what is wanted
void Brain::ChangeServoSignal()
{
    _servo_signal_left += copysign(1,_servo_signal_wanted_left - _servo_signal_left)*ACCELERATION;
    _servo_signal_right += copysign(1,_servo_signal_wanted_right - _servo_signal_right)*ACCELERATION;
    if(_servo_signal_left > UPPER_LIMIT){
        _servo_signal_left = UPPER_LIMIT;
    }
    if(_servo_signal_left < LOWER_LIMIT){
        _servo_signal_left = LOWER_LIMIT;
    }
    if(_servo_signal_right > UPPER_LIMIT){
        _servo_signal_right = UPPER_LIMIT;
    }
    if(_servo_signal_right < LOWER_LIMIT){
        _servo_signal_right = LOWER_LIMIT;
    }
    Serial.print("Servo left:");
    Serial.println(_servo_signal_left);
    Serial.print("Servo wanted left:");
    Serial.println(_servo_signal_wanted_left);
}

// -1 no read, else give distance to object
int Brain::ReadUltrasonic()
{
    short maximumRange = 400;
    short minimumRange = 0;

     digitalWrite(_pin_ultrasonic_trig, LOW);
     delayMicroseconds(2);

     digitalWrite(_pin_ultrasonic_trig, HIGH);
     delayMicroseconds(10);

     digitalWrite(_pin_ultrasonic_trig, LOW);
     float duration = pulseIn(_pin_ultrasonic_echo, HIGH);
     float distance = duration/58.2;
     if (distance >= maximumRange || distance <= minimumRange){
        distance = -1;
     }

     delay(50);

     return floor(distance);
}

// 1 No touch, 0 touch
bool Brain::ReadWhiskers(byte pin_nr)
{
    bool whiskers_state = digitalRead(pin_nr);
    return whiskers_state;
}

// 0 no read, 20-40 read based on distance
byte Brain::ReadIR(byte pin_reciever, byte pin_transmitter)
{
    delay(100);
    unsigned short frequencies[5] = {38000,39000,40000,41000,42000};
    byte distances[5] = {40,35,30,25,20};
    byte distance = 0;
    for(short i = 4; i >= 0; i--)
    {
        bool detection = Brain::irDetect(pin_transmitter, pin_reciever, frequencies[i]);
        if (!detection){
            distance = distances[i];
            break;
        }
    }
    return distance;
}

// 0 if detect based on frequency
bool Brain::irDetect(byte irLedPin, byte irReceiverPin, unsigned short frequency)
{
  tone(irLedPin, frequency, 8);
  delay(1);
  int ir = digitalRead(irReceiverPin);
  delay(10);
  return ir;
}
