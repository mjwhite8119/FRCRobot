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
     * Sets the wheel speed for the motors.  The wheel speed 
     * between 1 and -1, where 1 is full speed forward and -1
     * is full speed backwards.
     *
     * @param leftWheelSpeed - A wheel speed between 1 and -1
     * @param rightWheelSpeed - A wheel speed between 1 and -1
     */
    void setWheelSpeeds(const float leftWheelSpeed, 
                        const float rightWheelSpeed) 
    {  
      leftWheel.motor.setSpeed(leftWheelSpeed); 
      rightWheel.motor.setSpeed(rightWheelSpeed); 
    }

};

#endif // _DRIVE_TRAIN_H_