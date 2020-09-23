#ifndef _DRIVE_TRAIN_H_
#define _DRIVE_TRAIN_H_

#include "Wheel.h"

class DriveTrain
{
  public:
    
    // Constructor
    DriveTrain() {}

    const uint8_t leftWheelPinGroup = 0; // GPIO pin group. See config.h
    const uint8_t rightWheelPinGroup = 1; // GPIO pin group. See config.h
    
    Wheel leftWheel = Wheel(leftWheelPinGroup); // Attach the left wheel
    Wheel rightWheel = Wheel(rightWheelPinGroup); // Attach the right wheel

    /**
     * Sets the direction and wheel speed for the motors.
     *
     * @param leftDir  Direction for left motor.
     * @param rightDir Direction for right motor.
     * @param leftWheelSpeed The PWM value to send to the left motor.
     * @param rightWheelSpeed The PWM value to send to the right motor.
     */
    void setWheelSpeeds(const float leftWheelSpeed, 
                        const float rightWheelSpeed) 
    {  
      leftWheel.motor.applyPower(sgn(leftWheelSpeed), leftWheelSpeed); 
      rightWheel.motor.applyPower(rightWheelSpeed); 
    }

};

#endif // _DRIVE_TRAIN_H_