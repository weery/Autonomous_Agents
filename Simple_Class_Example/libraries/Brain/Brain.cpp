#include "Brain.h"

Brain::Brain()
{
}

Brain::~Brain(){
    
}

void Brain::InitializePin(int pin){
    pinMode(pin,OUTPUT);
    _pin=pin;
}

void Brain::Run()
{
    tone(_pin,1000,3000);
    digitalWrite(_pin,HIGH);
    delay(1000);
}