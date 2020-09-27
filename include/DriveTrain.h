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
     * @param timeOut - Time that the motor should run before stopping
     */
    void setWheelSpeeds(const int leftDir,
                        const int rightDir,
                        const float leftWheelSpeed, 
                        const float rightWheelSpeed,
                        const int timeOut) 
    {  
      leftWheel.motor.setSpeed(leftDir, leftWheelSpeed, timeOut); 
      rightWheel.motor.setSpeed(rightDir, rightWheelSpeed, timeOut); 
    }

    /**
     * Returns whether the motors are running
     */
    bool active() {
      if (leftWheel.motor.running || rightWheel.motor.running) {
        return true;
      } else {
        return false;
      }      
    }

    bool inActive() { return !active(); }

};

#endif // _DRIVE_TRAIN_H_