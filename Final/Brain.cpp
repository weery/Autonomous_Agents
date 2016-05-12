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

    _servo_signal_claw = 90;

    _servo_wheel_left.writeMicroseconds(_servo_signal_wheel_left);
    _servo_wheel_right.writeMicroseconds(_servo_signal_wheel_right);

    _servo_tower.write(_servo_signal_tower);

    _servo_claw.write(_servo_signal_claw);


    // Intialize states
    _current_state = STATE_TEST_SENSOR;

    _current_movement = STATE_FORWARD;

    _movement_action = ACTION_UNDECIDED;

    _current_behaviour = ROAM;

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
    byte ir_left_front_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_left_front,_pin_ir_transmitter_left_front);
    byte ir_right_front_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_right_front,_pin_ir_transmitter_right_front);

    byte ir_left_back_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_left_back,_pin_ir_transmitter_left_back);
    byte ir_right_back_distance_reading= Brain::ReadIrDistance(_pin_ir_reciever_right_back,_pin_ir_transmitter_right_back);

    bool whiskers_reading;
    // =digitalRead(_pin_whiskers);

    switch(_current_behaviour)
    {

        case LOCALIZE_BEACON:
            ir_left_front_reading = Brain::ReadIr(_pin_ir_reciever_left_front);
            ir_right_front_reading = Brain::ReadIr(_pin_ir_reciever_right_front);

            ir_left_back_reading = Brain::ReadIr(_pin_ir_reciever_left_back);
            ir_right_back_reading = Brain::ReadIr(_pin_ir_reciever_right_back);

            if (movement_time > 100)
            {
                movement_time = 0;
                _current_behaviour = ROAM;
                int random_heading = rand() % 5;
                if (random_heading == 0)
                    _current_movement = STATE_FORWARD;
                else if (random_heading == 1)
                    _current_movement = STATE_ROTATE_LEFT;
                else if (random_heading == 2)
                    _current_movement = STATE_ROTATE_RIGHT;
                else if (random_heading == 3)
                    _current_movement = STATE_FORWARD_LEFT;
                else if (random_heading == 4)
                    _current_movement = STATE_FORWARD_RIGHT;
            }
            else
            {
                if (ir_left_back_reading)
                    _current_movement = STATE_ROTATE_LEFT;
                else if (ir_right_back_reading)
                    _current_movement = STATE_ROTATE_RIGHT;
                else if (ir_right_front_reading && ir_left_front_reading)
                {
                    _current_behaviour = GO_TO_BEACON;
                    movement_time=0;
                }
                else if (ir_left_front_reading)
                    _current_movement = STATE_ROTATE_LEFT;
                else if (ir_right_front_reading)
                    _current_movement = STATE_ROTATE_RIGHT;

                movement_time++;
            }
            break;
        case GO_TO_BEACON:
            phototransistor_reading = Brain::ReadPhototransistor(_pin_phototransistor);

            if (phototransistor_reading < BLACK_PAPER_LIMIT)
            {
                movement_time = 0;
                _current_behaviour = LEAVE_CAN;
                break;
            }
            if (movement_time > 10)
            {
                _current_behaviour = LOCALIZE_BEACON;
                movement_time = 0;
                _current_movement = STATE_STOP;
            }
            else
            {
                _current_movement = STATE_FORWARD;
                movement_time++;
            }
            break;
        case LEAVE_CAN:
            _servo_signal_claw = MAX_ANGLE;
            if (movement_time > 10)
            {
                _current_behaviour = ROAM;
                movement_time=0;
                _current_movement = STATE_BACKWARD;
            }
            else
            {
                _current_movement = STATE_STOP;
                movement_time++;
            }

            break;
        case LOCALIZE_CAN:
            _current_movement = STATE_STOP;
            if (movement_time >= 19)
            {
                movement_time =0;
                _servo_signal_tower = MIDDLE_ANGLE;
                if (_can_reading < 255)
                {
                    _current_behaviour = HEAD_TO_CAN;
                }
                else
                {
                    _current_behaviour = ROAM;
                    int random_heading = rand() % 5;
                    if (random_heading == 0)
                        _current_movement = STATE_FORWARD;
                    else if (random_heading == 1)
                        _current_movement = STATE_ROTATE_LEFT;
                    else if (random_heading == 2)
                        _current_movement = STATE_ROTATE_RIGHT;
                    else if (random_heading == 3)
                        _current_movement = STATE_FORWARD_LEFT;
                    else if (random_heading == 4)
                        _current_movement = STATE_FORWARD_RIGHT;
                }
            }
            ultrasonic_lower_reading= Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
            ultrasonic_upper_reading= Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

            if (ultrasonic_lower_reading< _can_reading && abs(ultrasonic_lower_reading-ultrasonic_upper_reading)> ULTRASONIC_DIFF_MARGIN)
            {
                _can_angle = _servo_signal_tower;
                _can_reading = ultrasonic_lower_reading;
            }
            _servo_signal_tower+=5;
            movement_time++;
            break;
        case HEAD_TO_CAN:
            ultrasonic_lower_reading= Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
            if (movement_time > 20)
            {
                movement_time=0;
                _current_behaviour = LOCALIZE_CAN;
                _can_reading=255;
                _can_angle=MIDDLE_ANGLE;
                _servo_signal_tower = MIN_ANGLE;
            }
            if (abs(ultrasonic_lower_reading-_can_reading)<3)
            {
                movement_time =0;
                _current_behaviour = GO_TO_CAN;
            }
            else if (_can_angle>MIDDLE_ANGLE )
            {
                _current_movement = STATE_ROTATE_RIGHT;
            }
            else if (_can_angle< MIDDLE_ANGLE)
            {
                _current_movement = STATE_ROTATE_LEFT;
            }
            else
            {
                movement_time =0;
                _current_behaviour = GO_TO_CAN;
            }
            break;
        case GO_TO_CAN:
            _current_movement=STATE_FORWARD;
            whiskers_reading = digitalRead(_pin_whiskers);
            if (whiskers_reading)
            {
                movement_time =0;
                _current_behaviour = CATCH_CAN;
                _current_movement=STATE_STOP;
            }

            break;
        case CATCH_CAN:
            _current_movement=STATE_STOP;
            _servo_signal_claw = MIN_ANGLE;
            if (movement_time > 10)
            {
                _current_behaviour = LOCALIZE_BEACON;
                movement_time =0;
            }
            movement_time++;
            break;
        case ROAM:
            switch(_movement_action)
            {
                case ACTION_LOCKED:
                    {
                        movement_time++;
                        if (movement_time == 5)
                        {
                            _movement_action = ACTION_UNDECIDED;
                            movement_time = 0;
                        }
                        break;
                    }
                case ACTION_UNDECIDED:
                    {
                        Serial.println(ir_left_front_distance_reading);
                        bool leftDetected = ir_left_front_distance_reading>0;
                        bool rightDetected = ir_right_front_distance_reading>0;
                        if(leftDetected && rightDetected) {
                            int r = rand() % 1;
                            if (r >0){
                                _current_movement = STATE_ROTATE_LEFT;
                                _movement_action = ACTION_LOCKED;
                            }else{
                                _current_movement = STATE_ROTATE_RIGHT;
                                _movement_action = ACTION_LOCKED;
                            }
                        }else if(leftDetected){
                            _current_movement = STATE_ROTATE_RIGHT;
                        }else if(rightDetected){
                            _current_movement = STATE_ROTATE_LEFT;
                        }else{
                            _current_movement = STATE_FORWARD;
                        }
                        break;
                    }
            }
            break;
        //case ROAM:
            //whiskers_reading = digitalRead(_pin_whiskers);
            //if (movement_time>20)
            //{
                //movement_time =0;
                //_current_movement = STATE_STOP;
                //if (whiskers_reading)
                    //_current_behaviour = LOCALIZE_BEACON;
                //else
                //{
                    //_current_behaviour = LOCALIZE_CAN;
                    //_can_reading=255;
                    //_can_angle=MIDDLE_ANGLE;
                    //_servo_signal_tower = MIN_ANGLE;
                //}
            //}
            //else
            //{
                //// Random walk
                //movement_time++;
            //}
            //break;
        case TEST_SENSOR:
            whiskers_reading = digitalRead(_pin_whiskers);
            if (!whiskers_reading) {
                _servo_signal_claw = 135;
            } else {
                _servo_signal_claw = 90;
            }
            Serial.print("Whiskers reading: ");
            Serial.println(whiskers_reading);
            break;
    }

    /* _current_state
       switch(_current_state)
       {
       case STATE_TEST_SENSOR:
       _pin_servo_claw=45;
       _servo_claw.write(_pin_servo_claw);
       delay(1000);
       _update_counter+=1000;
       _pin_servo_claw=135;
       _servo_claw.write(_pin_servo_claw);
       Serial.println(digitalRead(_pin_whiskers));
       break;
       case STATE_FIND_SAFEZONE:
       {
       if (phototransistor_reading < BLACK_PAPER_LIMIT)
       {
       _current_movement = STATE_STOP;
       break;
       }
       }
       case STATE_ROAM:
       {
       switch(_movement_action)
       {
       case ACTION_LOCKED:
       {
       movement_time++;
       if (movement_time == 5)
       {
       _movement_action = ACTION_UNDECIDED;
       movement_time = 0;
       }
       break;
       }
       case ACTION_UNDECIDED:
       {

       bool leftDetected = Brain::ReadIrDistance(_pin_ir_reciever_left_front,_pin_ir_transmitter)>0;
       bool rightDetected = Brain::ReadIrDistance(_pin_ir_reciever_right_front,_pin_ir_transmitter)>0;
       if(leftDetected && rightDetected) {
       int r = rand() % 1;
       if (r >0){
       _current_movement = STATE_ROTATE_LEFT;
       _movement_action = ACTION_LOCKED;
       }else{
       _current_movement = STATE_ROTATE_RIGHT;
       _movement_action = ACTION_LOCKED;
       }
       }else if(leftDetected){
       _current_movement = STATE_ROTATE_RIGHT;
       }else if(rightDetected){
       _current_movement = STATE_ROTATE_LEFT;
       }else{
       _current_movement = STATE_FORWARD;
       }
       break;
       }
       }
       break;
       }
       case STATE_FIND_CAN:
       {
       _current_movement = STATE_STOP;
       const byte MAX_ANGLE = 135;
       const byte MIN_ANGLE = 45;
       const byte INCREMENT = 10;
       const byte N_INCREMENTS=(MAX_ANGLE-MIN_ANGLE)/INCREMENT;
       const byte ULTRASONIC_DIFF_MARGIN = 6;

       byte min_reading = 255;
       byte min_reading_angle=90;
    _servo_tower.write(MIN_ANGLE);
    delay(1000);
    _update_counter+=1000;
    for (byte i = 0; i<N_INCREMENTS;i++ )
    {
        byte angle = MIN_ANGLE + i*(MAX_ANGLE-MIN_ANGLE)/(N_INCREMENTS-1);
        _servo_tower.write(angle);
        delay(30);
        _update_counter+=30;
        ultrasonic_lower_reading = Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
        ultrasonic_upper_reading = Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

        if (ultrasonic_lower_reading<min_reading && abs(ultrasonic_upper_reading-ultrasonic_lower_reading)>ULTRASONIC_DIFF_MARGIN )
        {
            min_reading = ultrasonic_lower_reading;
            min_reading_angle= angle;
            Serial.println(min_reading_angle);

        }
    }
    for (byte i = N_INCREMENTS; i>0;i--)
    {
        byte angle = MIN_ANGLE + (i-1)*(MAX_ANGLE-MIN_ANGLE)/(N_INCREMENTS-1);
        _servo_tower.write(angle);
        delay(30);
        _update_counter+=30;
        ultrasonic_lower_reading = Brain::ReadUltrasonic2Pin(_pin_ultrasonic_lower_echo,_pin_ultrasonic_lower_trig);
        ultrasonic_upper_reading = Brain::ReadUltrasonic1Pin(_pin_ultrasonic_upper);

        if (ultrasonic_lower_reading<min_reading && abs(ultrasonic_upper_reading-ultrasonic_lower_reading)>ULTRASONIC_DIFF_MARGIN)
        {
            min_reading = ultrasonic_lower_reading;
            min_reading_angle= angle;
            Serial.println(min_reading_angle);

        }
    }

    _servo_tower.write(min_reading_angle);
    Serial.println(min_reading_angle);
    Serial.println(min_reading);
    delay(1000);
    _update_counter+=1000;
    break;
}
}
*/

// ADD COLLISION AVOIDANCE
switch(_current_movement)
{
    case STATE_ROTATE_LEFT_SLOWLY:
        _servo_signal_wheel_left = 1490;
        _servo_signal_wheel_right = 1490;
        break;
    case STATE_ROTATE_RIGHT_SLOWLY:
        _servo_signal_wheel_left = 1510;
        _servo_signal_wheel_right = 1510;
    case STATE_ROTATE_LEFT:
        _servo_signal_wheel_left = 1450;
        _servo_signal_wheel_right = 1450;
        break;
    case STATE_ROTATE_RIGHT:
        _servo_signal_wheel_left = 1550;
        _servo_signal_wheel_right = 1550;
        break;
    case STATE_FORWARD:
        _servo_signal_wheel_left = 1550;
        _servo_signal_wheel_right = 1450;
        break;
    case STATE_BACKWARD:
        _servo_signal_wheel_left = 1450;
        _servo_signal_wheel_right = 1550;
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
}

//Serial.print("Current Behaviour: ");
//Serial.println(_current_behaviour);

//Serial.print("Current Movement: ");
//Serial.println(_current_movement);

_servo_signal_wheel_left = Brain::Clamp(_servo_signal_wheel_left,MAX_SIGNAL,MIN_SIGNAL);
_servo_signal_wheel_right = Brain::Clamp(_servo_signal_wheel_right,MAX_SIGNAL,MIN_SIGNAL);

_servo_wheel_left.writeMicroseconds(_servo_signal_wheel_left);
_servo_wheel_right.writeMicroseconds(_servo_signal_wheel_right);

//Serial.print("Before clamp");
//Serial.println(_servo_signal_claw);
_servo_signal_claw = Brain::Clamp(_servo_signal_claw,MAX_ANGLE,MIN_ANGLE);
//Serial.print("After Clamp");
//Serial.println(_servo_signal_claw);

_servo_claw.write(_servo_signal_claw);

_servo_signal_tower = Brain::Clamp(_servo_signal_tower,MAX_ANGLE,MIN_ANGLE);

_servo_tower.write(_servo_signal_tower);


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
    byte distances[4] = {26,21,15,10};
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
    return ir;
}


byte Brain::ReadPhototransistor(byte pin_phototransistor)
{
    return analogRead(pin_phototransistor);
}
