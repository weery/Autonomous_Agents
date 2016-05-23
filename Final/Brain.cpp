#include "Brain.h"

void Brain::InitializePins(byte pin_servo_wheel_left,byte pin_servo_wheel_right,
        byte pin_servo_tower, byte pin_servo_claw,
        byte pin_ultrasonic_lower_echo,byte pin_ultrasonic_lower_trig,
        byte pin_ultrasonic_upper, byte pin_ir_reciever_left_front,
        byte pin_ir_reciever_right_front ,byte pin_ir_reciever_left_back,
        byte pin_ir_reciever_right_back,byte pin_ir_transmitter_left_front,
        byte pin_ir_transmitter_right_front, byte pin_ir_transmitter_left_back,
        byte pin_ir_transmitter_right_back, byte pin_phototransistor, byte pin_whiskers,
        byte pin_led)
{
    // Assign all pins
    _pin_servo_wheel_left= pin_servo_wheel_left;
    _pin_servo_wheel_right= pin_servo_wheel_right,

    _pin_servo_tower= pin_servo_tower;
    _pin_servo_claw= pin_servo_claw;

    _pin_ultrasonic_lower_echo= pin_ultrasonic_lower_echo;
    _pin_ultrasonic_lower_trig= pin_ultrasonic_lower_trig;

    _pin_ultrasonic_upper= pin_ultrasonic_upper;

    _pin_ir_reciever_left_front= pin_ir_reciever_left_front;
    _pin_ir_reciever_right_front= pin_ir_reciever_right_front;

    _pin_ir_reciever_left_back= pin_ir_reciever_left_back;
    _pin_ir_reciever_right_back= pin_ir_reciever_right_back;

    _pin_ir_transmitter_left_front = pin_ir_transmitter_left_front;
    _pin_ir_transmitter_right_front = pin_ir_transmitter_right_front;

    _pin_ir_transmitter_left_back = pin_ir_transmitter_left_back;
    _pin_ir_transmitter_right_back = pin_ir_transmitter_right_back;

    _pin_phototransistor= pin_phototransistor;

    _pin_led = pin_led;
    pinMode(_pin_led,OUTPUT);

    _pin_whiskers = pin_whiskers;
    pinMode(_pin_whiskers,INPUT);

    // Assign pin input/output

    pinMode(_pin_ir_reciever_left_front,INPUT);
    pinMode(_pin_ir_reciever_right_front,INPUT);

    pinMode(_pin_ir_reciever_left_back,INPUT);
    pinMode(_pin_ir_reciever_right_back,INPUT);

    pinMode(_pin_ir_transmitter_left_front,OUTPUT);
    pinMode(_pin_ir_transmitter_right_front,OUTPUT);
    pinMode(_pin_ir_transmitter_left_back,OUTPUT);
    pinMode(_pin_ir_transmitter_right_back,OUTPUT);

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

    _servo_signal_claw = MIN_ANGLE;

    _servo_wheel_left.writeMicroseconds(_servo_signal_wheel_left);
    _servo_wheel_right.writeMicroseconds(_servo_signal_wheel_right);

    _servo_tower.write(_servo_signal_tower);

    _servo_claw.write(_servo_signal_claw);


    // Intialize states
    _current_state = STATE_TEST_SENSOR;

    _current_movement = STATE_FORWARD;

    _movement_action = ACTION_UNDECIDED;

    _current_behaviour = LOCALIZE_CAN;

    delay(100);
}

void Brain::Run()
{
    _update_counter=0;

    // Example of how to get the different readings
    int ultrasonic_lower_reading;
    //= Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
    int ultrasonic_upper_reading;
    //= Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

    float phototransistor_reading;
    // = Brain::ReadPhototransistor(_pin_phototransistor);

    bool ir_left_front_reading;
    //    = Brain::ReadIr(_pin_ir_reciever_left_front);
    bool ir_right_front_reading;
    //= Brain::ReadIr(_pin_ir_reciever_right_front);

    bool ir_left_back_reading;
    //= Brain::ReadIr(_pin_ir_reciever_left_back);
    bool ir_right_back_reading;
    //= Brain::ReadIr(_pin_ir_reciever_right_back);

    // Always initialized as they are used for collision detection which is always imminent

    byte ir_left_back_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_left_back,_pin_ir_transmitter_left_back);
    byte ir_right_back_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_right_back,_pin_ir_transmitter_right_back);

    bool whiskers_reading;

    if (CollisionTimer>0)
    {
        CollisionTimer--;
        if(CollisionTimer == 5 && _current_movement == STATE_BACKWARD)
        {
            if (rand() % 2 == 0)
            _current_movement = STATE_ROTATE_LEFT;
            else
                _current_movement = STATE_ROTATE_RIGHT;

            Brain::ChangeWheelServos();

        }
    }
    else
    {
        switch(_current_behaviour)
        {
            case LOCALIZE_BEACON:
                Brain::LocalizeBeacon();
                break;
            case HEAD_TO_BEACON:
                Brain::HeadToBeacon();
                break;
            case GO_TO_BEACON:
                if (Brain::AvoidCollision(COLLISION_DISTANCE_SHORT))
                {
                    CollisionTimer = CollisionTimer + 5;
                    _current_behaviour = LOCALIZE_BEACON;
                    movement_time = 0;
                    break;
                }
                Brain::GoToBeacon();
                break;
            case LEAVE_CAN:
                Brain::LeaveCan();
                break;
            case LOCALIZE_CAN:
                Brain::LocalizeCan();
                break;
            case HEAD_TO_CAN:
                Brain::HeadToCan();
                break;
            case GO_TO_CAN:
                if (Brain::AvoidCollision(COLLISION_DISTANCE_SHORT))
                {
                    CollisionTimer = CollisionTimer + 5;
                    _current_behaviour = LOCALIZE_CAN;
                    Brain::GoToLocalizeCan();
                    break;
                }
                Brain::GoToCan();
                break;
            case LEAVE_SAFE_ZONE:
                Brain::LeaveSafeZone();
                break;
            case CATCH_CAN:
                Brain::CatchCan();
                break;
            case ROAM:
                if (Brain::AvoidCollision(COLLISION_DISTANCE_SHORT))
                {
                    CollisionTimer = CollisionTimer + 5;
                    break;
                }
                Brain::Roam();
                break;
            case TEST_SENSOR:
                int ultrasonic_lower_reading= Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
                delay(10);
                int ultrasonic_upper_reading= Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

                Serial.println(ultrasonic_lower_reading-ultrasonic_upper_reading);
                break;
        }
    }

    byte remaining_delay=UPDATE_DELAY-_update_counter;
    delay(remaining_delay);
}

void Brain::LeaveSafeZone()
{
    if (movement_time < 5)
    {
        if (_current_movement != STATE_BACKWARD)
        {
            _current_movement = STATE_BACKWARD;
            Brain::ChangeWheelServos();
        }
    }
    else if (movement_time < 10)
    {
        if (_current_movement != STATE_ROTATE_LEFT && _current_movement != STATE_ROTATE_RIGHT)
        {
            int r = rand() % 2;
            if (r ==0)
                _current_movement = STATE_ROTATE_LEFT;
            else
                _current_movement = STATE_ROTATE_RIGHT;
            Brain::ChangeWheelServos();
        }
    }
    else
    {
        movement_time=0;
        _current_behaviour = ROAM;
        return;
    }
    movement_time++;
}

void Brain::LocalizeBeacon()
{
    bool ir_left_front_reading = Brain::ReadIr(_pin_ir_reciever_left_front);
    bool ir_right_front_reading = Brain::ReadIr(_pin_ir_reciever_right_front);

    bool ir_left_back_reading = Brain::ReadIr(_pin_ir_reciever_left_back);
    bool ir_right_back_reading = Brain::ReadIr(_pin_ir_reciever_right_back);

    Serial.print(ir_left_back_reading);
    Serial.print(ir_left_front_reading);
    Serial.print(ir_right_back_reading);
    Serial.println(ir_right_front_reading);
    if (_current_movement != STATE_STOP)
    {
        _current_movement = STATE_STOP;
        Brain::ChangeWheelServos();
    }

    if (movement_time > MAX_LOCALIZING_BEACON)
    {
        movement_time = 0;
        _current_behaviour = ROAM;
    }
    else
    {
        if (ir_left_back_reading){
            if (_current_movement != STATE_ROTATE_LEFT)
            {
                _current_movement = STATE_ROTATE_LEFT;
                Brain::ChangeWheelServos();
            }
        }
        else if (ir_right_back_reading){
            if (_current_movement != STATE_ROTATE_RIGHT)
            {
                _current_movement = STATE_ROTATE_RIGHT;
                Brain::ChangeWheelServos();
            }
        }
        else if (ir_right_front_reading && ir_left_front_reading)
        {
            _current_behaviour = GO_TO_BEACON;
            movement_time=0;
        }
        else if (ir_left_front_reading){
            if (_current_movement != STATE_ROTATE_LEFT_SLOWLY)
            {
                _current_movement = STATE_ROTATE_LEFT_SLOWLY;
                Brain::ChangeWheelServos();
            }
        }
        else if (ir_right_front_reading){
            if (_current_movement != STATE_ROTATE_RIGHT_SLOWLY)
            {
                _current_movement = STATE_ROTATE_RIGHT_SLOWLY;
                Brain::ChangeWheelServos();
            }
        }
        else
        {
            movement_time++;
            return;
        }
        _current_behaviour = HEAD_TO_BEACON;
        movement_time -= movement_time % 7;
        movement_time++;
    }
}

void Brain::HeadToBeacon()
{
    if (movement_time % 7 == 0)
    {
        _current_behaviour = LOCALIZE_BEACON;
    }
    movement_time++;
}

void Brain::GoToBeacon()
{
    Brain::IsAtBeacon();

    if (movement_time > 10)
    {
        _current_behaviour = LOCALIZE_BEACON;
        movement_time = 0;
        if (_current_movement != STATE_STOP)
            {
                _current_movement = STATE_STOP;
                Brain::ChangeWheelServos();
            }
    }
    else
    {
        if (_current_movement != STATE_FORWARD)
            {
                _current_movement = STATE_FORWARD;
                Brain::ChangeWheelServos();
            }
        movement_time++;
    }
}
bool Brain::IsAtBeacon()
{
    digitalWrite(_pin_led,HIGH);
    delay(10);
    float phototransistor_reading = Brain::ReadPhototransistor(_pin_phototransistor);
    digitalWrite(_pin_led,LOW);
    if (phototransistor_reading < BLACK_PAPER_LIMIT)
    {
        movement_time = 0;
        _current_behaviour = LEAVE_CAN;
        return true;
    }
    return false;
}

void Brain::LeaveCan()
{
    if(_servo_signal_claw!=MIN_ANGLE)
    {
        _servo_signal_claw = MIN_ANGLE;
        Brain::ChangeClawServo();
    }
    if (movement_time > 10)
    {
        _current_behaviour = LEAVE_SAFE_ZONE;
        movement_time=0;
        has_can = false;
    }
    else
    {
        if (_current_movement != STATE_STOP)
            {
                _current_movement = STATE_STOP;
                Brain::ChangeWheelServos();
            }
        movement_time++;
    }
}

void Brain::LocalizeCan()
{
    if (movement_time == 0)
    {
        _servo_signal_tower = MIN_ANGLE;
        Brain::ChangeTowerServo();
        delay(20);
    }
    if (_current_movement != STATE_STOP)
    {
        _current_movement = STATE_STOP;
        Brain::ChangeWheelServos();
    }
    if (movement_time >= 19)
    {
        movement_time =0;
        _servo_signal_tower = MIDDLE_ANGLE;
        Brain::ChangeTowerServo();
        if (_can_reading < 255)
        {
            _current_behaviour = HEAD_TO_CAN;
        }
        else
        {
            _current_behaviour = ROAM;
        }
    }
    int ultrasonic_lower_reading= Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
    delay(10);
    int ultrasonic_upper_reading= Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

    Serial.println(ultrasonic_lower_reading-ultrasonic_upper_reading);
    if (ultrasonic_lower_reading< _can_reading &&
        abs(ultrasonic_lower_reading-ultrasonic_upper_reading)> ULTRASONIC_DIFF_MARGIN &&
        ultrasonic_lower_reading < 50)
    {
        _can_angle = _servo_signal_tower;
        _can_reading = ultrasonic_lower_reading;
    }
    _servo_signal_tower+=5;
    Brain::ChangeTowerServo();
    movement_time++;
}

void Brain::GoToLocalizeCan()
{
  movement_time=0;
  _can_reading=255;
  _can_angle=MIDDLE_ANGLE;
  _servo_signal_tower=MIN_ANGLE;
}

void Brain::HeadToCan()
{
    int ultrasonic_lower_reading= Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
    if (movement_time > 20)
    {
        movement_time=0;
        _current_behaviour = LOCALIZE_CAN;
        Brain::GoToLocalizeCan();
    _current_behaviour = LOCALIZE_CAN;
        return;
    }

    if (abs(ultrasonic_lower_reading - _can_reading)<3)
    {
        movement_time =0;
        _current_behaviour = GO_TO_CAN;
    }
    else if (_can_angle>MIDDLE_ANGLE )
    {
        if (_current_movement != STATE_ROTATE_LEFT_SLOWLY)
        {
            _current_movement = STATE_ROTATE_LEFT_SLOWLY;
            Brain::ChangeWheelServos();
        }
    }
    else if (_can_angle< MIDDLE_ANGLE)
    {
        if (_current_movement != STATE_ROTATE_RIGHT_SLOWLY)
        {
            _current_movement = STATE_ROTATE_RIGHT_SLOWLY;
            Brain::ChangeWheelServos();
        }
    }
    else
    {
        movement_time =0;
        _current_behaviour = GO_TO_CAN;
    }
    movement_time++;
}

void Brain::GoToCan()
{
    if (movement_time > MAX_MOVEMENT_TOWARDS_CAN) {
        movement_time = 0;
        Brain::GoToLocalizeCan();
        _current_behaviour = LOCALIZE_CAN;
        return;
    }
    movement_time++;

    if (_current_movement != STATE_FORWARD)
    {
        _current_movement = STATE_FORWARD;
        Brain::ChangeWheelServos();
    }

    Brain::CheckHasCan();
}

bool Brain::CheckHasCan()
{
    bool whiskers_reading = !digitalRead(_pin_whiskers);

    if (whiskers_reading)
    {
        movement_time =0;
        _current_behaviour = CATCH_CAN;
         if (_current_movement != STATE_STOP)
          {
              _current_movement = STATE_STOP;
              Brain::ChangeWheelServos();
          }
    }
    return whiskers_reading;
}

void Brain::CatchCan()
{
    if (_current_movement != STATE_STOP)
    {
        _current_movement = STATE_STOP;
        Brain::ChangeWheelServos();
    }
    if(_servo_signal_claw!=MAX_ANGLE)
    {
        _servo_signal_claw = MAX_ANGLE;
        Brain::ChangeClawServo();
    }
    if (movement_time > 3)
    {
        _current_behaviour = LOCALIZE_BEACON;
        movement_time =0;
        has_can = true;
    }
    movement_time++;
}

void Brain::Roam()
{
    if (movement_time < 20)
    {
        if ((!has_can && Brain::CheckHasCan()) ||
            (has_can && Brain::IsAtBeacon()))
        {
           return;
        }
        int r = rand() % 32;
        if (r==2)
        {
            if (_current_movement != STATE_ROTATE_LEFT)
            {
                _current_movement = STATE_ROTATE_LEFT;
                Brain::ChangeWheelServos();
            }
        }
        else if (r==3)
        {
            if (_current_movement != STATE_ROTATE_RIGHT)
            {
                _current_movement = STATE_ROTATE_RIGHT;
                Brain::ChangeWheelServos();
            }
        }
        else
        {
            if (_current_movement != STATE_FORWARD)
            {
                _current_movement = STATE_FORWARD;
                Brain::ChangeWheelServos();
            }
        }
        movement_time++;
    }
    else
    {
        if (has_can)
        {
            _current_behaviour = LOCALIZE_BEACON;
        }
        else
        {
            _current_behaviour = LOCALIZE_CAN;
            Brain::GoToLocalizeCan();
            Brain::ChangeTowerServo();
        }
        movement_time = 0;
        if (_current_movement != STATE_STOP)
        {
            _current_movement = STATE_STOP;
            Brain::ChangeWheelServos();
        }
    }
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

unsigned short Brain::Clamp(unsigned short val, unsigned short max, unsigned short min)
{
    if (val > max )
        return max;
    else if (val < min)
        return min;
    return val;
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
    _update_counter+=1;
    bool ir = digitalRead(pin_reciever);
    delay(7);
    _update_counter+=1;
    return ir;
}

byte Brain::ReadIrDistance(byte pin_reciever, byte pin_transmitter)
{
    unsigned short frequencies[4] = {38000,40500,45000,52000};
    //byte distances[4] = {26,21,15,10};
    byte distances[4] = {COLLISION_DISTANCE_LONG,COLLISION_DISTANCE_SEMILONG,COLLISION_DISTANCE_MIDDLE,COLLISION_DISTANCE_SHORT};
    byte distance = 0;
    for(short i = 3; i >= 0; i--)
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
    return !ir;
}


float Brain::ReadPhototransistor(byte pin_phototransistor)
{
    return analogRead(pin_phototransistor);
}

bool Brain::AvoidCollision(byte distance)
{
    byte ir_left_front_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_left_front,_pin_ir_transmitter_left_front);
    byte ir_right_front_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_right_front,_pin_ir_transmitter_right_front);
    bool leftDetected = (ir_left_front_distance_reading <= distance) && (ir_left_front_distance_reading > 0);
    bool rightDetected = (ir_right_front_distance_reading <= distance) && (ir_right_front_distance_reading > 0);
    int ultrasonic_upper_reading = Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);
    CollisionTimer = 0;
    if (ultrasonic_upper_reading < MIN_PINGSENSOR_READING)
    {
            if (_current_movement != STATE_BACKWARD)
            {
                _current_movement = STATE_BACKWARD;
                Brain::ChangeWheelServos();
            }
        CollisionTimer = 5;
        return true;
    }
    if (leftDetected && rightDetected)
    {
        int r = rand () % 2;
        if (r==0)
        {
            if (_current_movement != STATE_ROTATE_LEFT)
            {
                _current_movement = STATE_ROTATE_LEFT;
                Brain::ChangeWheelServos();
            }
        }
        else
        {
            if (_current_movement != STATE_ROTATE_RIGHT)
            {
                _current_movement = STATE_ROTATE_RIGHT;
                Brain::ChangeWheelServos();
            }
        }
        return true;
    }
    else if(leftDetected){
        if (_current_movement != STATE_FORWARD_RIGHT)
        {
            _current_movement = STATE_FORWARD_RIGHT;
            Brain::ChangeWheelServos();
        }
        return true;
    }else if(rightDetected){
        if (_current_movement != STATE_FORWARD_LEFT)
        {
            _current_movement = STATE_FORWARD_LEFT;
            Brain::ChangeWheelServos();
        }
        return true;
    }
    return false;
}


void Brain::ChangeWheelServos()
{
    switch(_current_movement)
    {
    case STATE_ROTATE_LEFT_SLOWLY:
        _servo_signal_wheel_left = MIDDLE_SIGNAL-15;
        _servo_signal_wheel_right = MIDDLE_SIGNAL-15;
        break;
    case STATE_ROTATE_RIGHT_SLOWLY:
        _servo_signal_wheel_left = MIDDLE_SIGNAL+15;
        _servo_signal_wheel_right = MIDDLE_SIGNAL+15;
        break;
    case STATE_ROTATE_LEFT:
        _servo_signal_wheel_left = 1450;
        _servo_signal_wheel_right = 1450;
        break;
    case STATE_ROTATE_RIGHT:
        _servo_signal_wheel_left = 1550;
        _servo_signal_wheel_right = 1550;
        break;
    case STATE_FORWARD:
        _servo_signal_wheel_left = MAX_SIGNAL;
        _servo_signal_wheel_right = MIN_SIGNAL;
        break;
    case STATE_BACKWARD:
        _servo_signal_wheel_left = MIN_SIGNAL;
        _servo_signal_wheel_right = MAX_SIGNAL;
        break;
    case STATE_STOP:
        _servo_signal_wheel_left = 1500;
        _servo_signal_wheel_right = 1500;
        break;
    case STATE_BACKWARD_LEFT:
        _servo_signal_wheel_left = 1450;
        _servo_signal_wheel_right = 1600;
        break;
    case STATE_BACKWARD_RIGHT:
        _servo_signal_wheel_left = 1400;
        _servo_signal_wheel_right = 1550;
        break;
    case STATE_FORWARD_RIGHT:
        _servo_signal_wheel_left = MAX_SIGNAL;
        _servo_signal_wheel_right = MIN_SIGNAL + 75;
        break;
    case STATE_FORWARD_LEFT:
        _servo_signal_wheel_left = MAX_SIGNAL - 75;
        _servo_signal_wheel_right = MIN_SIGNAL;
        break;
    }

    _servo_signal_wheel_left = Brain::Clamp(_servo_signal_wheel_left,MAX_SIGNAL,MIN_SIGNAL);
    _servo_signal_wheel_right = Brain::Clamp(_servo_signal_wheel_right,MAX_SIGNAL,MIN_SIGNAL);
    _servo_wheel_left.writeMicroseconds(_servo_signal_wheel_left);
    _servo_wheel_right.writeMicroseconds(_servo_signal_wheel_right);

}

void Brain::ChangeClawServo()
{
    _servo_signal_claw = Brain::Clamp(_servo_signal_claw,MAX_ANGLE,MIN_ANGLE);
    _servo_claw.write(_servo_signal_claw);
}

void Brain::ChangeTowerServo()
{
    _servo_signal_tower = Brain::Clamp(_servo_signal_tower,MAX_ANGLE,MIN_ANGLE);

    _servo_tower.write(_servo_signal_tower);
}

