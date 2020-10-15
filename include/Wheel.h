#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "DCMotor.h"

class Wheel
{ 
  public:   

    /**
     * Constructor for powered wheels.
     */ 
    Wheel() {} // Default constructor
    
    /**
     * Constructor for powered wheels.  The pin group is passed 
     * onto the motor to connect the GPIO pins.
     * 
     * @param pinGroup  GPIO pin group for this wheel.
     * 
     */ 
    Wheel(const uint8_t pinGroup) 
      :motor(pinGroup) {}

    // -------------------- Member variables -----------------  
    DCMotor motor;

    float diameter = 0.057; // wheel diameter in meters
};

#endif // _WHEEL_H_