#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "DCMotor.h"

class Wheel
{ 
  public:   

    // Default constructor
    Wheel() {}
    
    /**
     * Constructor for powered wheels.  The pin group is passed 
     * onto the motor to connect the GPIO pins.
     * 
     * @param pinGroup  GPIO pin group for this wheel.
     * 
     */ 
    Wheel(const uint8_t pinGroup) 
      :motor(pinGroup) {}

    /**
     * Gets the current linear position of the wheel since
     * robot startup
     * 
     * @return current linear position of the wheel
     */
    float currentPosition() const {
      return motor.encoder.pulses * distancePerPulse;
    }

    /**
     * Gets the number of encoder pulses per second to 
     * calulate the linear velocity of the wheel.
     * 
     * @return linear velocity per second of the wheel.
     */
    float getVelocityPerSecond() 
      {return (distancePerPulse * motor.getCurrentPulsesPerSecond());}  

    // -------------------- Member variables -----------------  
    DCMotor motor;
    const float diameter = 0.066; // wheel diameter in meters
    const float distancePerRevolution = PI * diameter; // Linear distance per/wheel rovolution
    const float distancePerPulse = distancePerRevolution / motor.encoder.PPR; // distance per/encoder pulse

};

#endif // _WHEEL_H_