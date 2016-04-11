#ifndef Brain_h
#define Brain_h

#include "Arduino.h"

class Brain { 
public:
	Brain();
	~Brain();
    void InitializePin(int pin);
    void Run();
private:
    byte _pin;
};

#endif