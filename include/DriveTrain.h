#ifndef _DRIVE_TRAIN_H_
#define _DRIVE_TRAIN_H_

#include "Wheel.h"

class DriveTrain
{
  public:
    
    // Constructor
    DriveTrain() {}

    const uint8_t leftWheelPinGroup = 0; // GPIO pin group config.h
    const uint8_t rightWheelPinGroup = 1; // GPIO pin group config.h
    
    Wheel leftWheel = Wheel(leftWheelPinGroup);
    Wheel rightWheel = Wheel(rightWheelPinGroup);

    void setWheelSpeeds(const int leftDir,
                        const int rightDir,
                        const float leftWheelSpeed, 
                        const float rightWheelSpeed) 
    {  
      leftWheel.motor.applyPower(leftDir, leftWheelSpeed); 
      rightWheel.motor.applyPower(rightDir, rightWheelSpeed); 
    }

};

#endif // _DRIVE_TRAIN_H_