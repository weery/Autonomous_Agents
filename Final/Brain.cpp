#include "Brain.h"

void Brain::InitializePins(byte pin_servo_wheel_left,byte pin_servo_wheel_right,
        byte pin_servo_tower, byte pin_servo_claw,
        byte pin_ultrasonic_lower_echo,byte pin_ultrasonic_lower_trig,
        byte pin_ultrasonic_upper, byte pin_ir_reciever_left_front,
        byte pin_ir_reciever_right_front ,byte pin_ir_reciever_left_back,
        byte pin_ir_reciever_right_back,byte pin_ir_transmitter_left_front,
        byte pin_ir_transmitter_right_front, byte pin_ir_transmitter_left_back,
        byte pin_ir_transmitter_right_back, byte pin_phototransistor, byte pin_whiskers)
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

    byte phototransistor_reading;
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
    // =digitalRead(_pin_whiskers);
    //Serial.print("Current Behaviour: ");
    //Serial.println(_current_behaviour);
    Serial.println(CollisionTimer);
    if (CollisionTimer>0)
    {
        CollisionTimer--;
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
                if (Brain::AvoidCollision(COLLISION_DISTANCE_MIDDLE))
                {
                    CollisionTimer = 10;
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
                if (Brain::AvoidCollision(COLLISION_DISTANCE_MIDDLE))
                {
                    CollisionTimer = 10;
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
                if (Brain::AvoidCollision(COLLISION_DISTANCE_SEMILONG))
                {
                    CollisionTimer = 10;
                    break;
                }
                Brain::Roam();
                break;
            case TEST_SENSOR:
                whiskers_reading = !digitalRead(_pin_whiskers);
                //if (!whiskers_reading) {
                    //_servo_signal_claw = 135;
                //} else {
                    //_servo_signal_claw = 90;
                //}
                Serial.print("Whiskers reading: ");
                Serial.println(whiskers_reading);
                break;
        }
    }

    byte remaining_delay=UPDATE_DELAY-_update_counter;
    delay(remaining_delay);
}

void Brain::LeaveSafeZone()
{    
    if (movement_time<10)
    {
        if (_current_movement != STATE_BACKWARD)
        {
            _current_movement = STATE_BACKWARD;
            Brain::ChangeWheelServos();
        }
    }
    else if (movement_time < 15)
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
    }
    movement_time++;
}

void Brain::LocalizeBeacon()
{
    bool ir_left_front_reading = Brain::ReadIr(_pin_ir_reciever_left_front);
    bool ir_right_front_reading = Brain::ReadIr(_pin_ir_reciever_right_front);

    bool ir_left_back_reading = Brain::ReadIr(_pin_ir_reciever_left_back);
    bool ir_right_back_reading = Brain::ReadIr(_pin_ir_reciever_right_back);

    if (_current_movement != STATE_STOP)
    {
        _current_movement = STATE_STOP;
        Brain::ChangeWheelServos();
    }

    if (movement_time > MAX_LOCALIZING_TIME)
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
            if (_current_movement != STATE_ROTATE_LEFT)
            {
                _current_movement = STATE_ROTATE_LEFT;
                Brain::ChangeWheelServos();
            }
        }
        else if (ir_right_front_reading){
            if (_current_movement != STATE_ROTATE_RIGHT)
            {
                _current_movement = STATE_ROTATE_RIGHT;
                Brain::ChangeWheelServos();
            }
        }
        else
        {
            movement_time++;
            return;
        }
        _current_behaviour = HEAD_TO_BEACON;
        movement_time++;
    }
}

void Brain::HeadToBeacon()
{
    if (movement_time % 20 == 0)
    {
        _current_behaviour = LOCALIZE_BEACON;
    }
    movement_time++;
}

void Brain::GoToBeacon()
{
    byte phototransistor_reading = Brain::ReadPhototransistor(_pin_phototransistor);

    if (phototransistor_reading < BLACK_PAPER_LIMIT)
    {
        movement_time = 0;
        _current_behaviour = LEAVE_CAN;
        return;
    }
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
    int ultrasonic_upper_reading= Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

    if (ultrasonic_lower_reading< _can_reading && abs(ultrasonic_lower_reading-ultrasonic_upper_reading)> ULTRASONIC_DIFF_MARGIN)
    {
        _can_angle = _servo_signal_tower;
        _can_reading = ultrasonic_lower_reading;
        Serial.print("Initial reading: ");
        Serial.println(_can_reading);
        Serial.println(_can_angle);
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
    Serial.println(abs(ultrasonic_lower_reading - _can_reading));

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
    if (movement_time > 20) {
        movement_time = 0;
        Brain::GoToLocalizeCan();
        Serial.println("I got here:D");
        return;
    }
    movement_time++;
    
    if (_current_movement != STATE_FORWARD)
    {
        _current_movement = STATE_FORWARD;
        Brain::ChangeWheelServos();
    }
    bool whiskers_reading = !digitalRead(_pin_whiskers);
    Serial.println("Can I sense it?");

    if (whiskers_reading)
    {
        Serial.println("I can sense it;)");
        movement_time =0;
        _current_behaviour = CATCH_CAN;
         if (_current_movement != STATE_STOP)
          {
              _current_movement = STATE_STOP;
              Brain::ChangeWheelServos();
          }
    }
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
        int r = rand() % 32;
        Serial.print("Roam value:");
        Serial.println(r);
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


byte Brain::ReadPhototransistor(byte pin_phototransistor)
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
    if (ultrasonic_upper_reading < MIN_PINGSENSOR_READING)
    {
        Serial.print("Upper reading");
        Serial.println(ultrasonic_upper_reading);
        if (_current_movement != STATE_BACKWARD_LEFT)
        {
            _current_movement = STATE_BACKWARD_LEFT;
            Brain::ChangeWheelServos();
        }
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
        _servo_signal_wheel_left = 1490;
        _servo_signal_wheel_right = 1490;
        break;
    case STATE_ROTATE_RIGHT_SLOWLY:
        _servo_signal_wheel_left = 1510;
        _servo_signal_wheel_right = 1510;
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
    Serial.print("left wheel: ");
    Serial.println(_servo_signal_wheel_left);
    Serial.print("right wheel: ");
    Serial.println(_servo_signal_wheel_right);
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

