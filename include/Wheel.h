#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "DCMotor.h"

class Wheel
{ 
  public:   

    // Default constructor
    Wheel() {}
    
    // Constructor for powered wheels   
    Wheel(const uint8_t pinGroup) 
      :motor(pinGroup) {}

    // -------------------- Member variables -----------------  
    DCMotor motor;
    float diameter = 0.057; // wheel diameter in meters
};

#endif // _WHEEL_H_