     _current_state
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
