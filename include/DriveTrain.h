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
     * @param leftWheelSpeed - Left wheel speed between -1 and +1
     * @param rightWheelSpeed - Right wheel speed between -1 and +1
     * @param timeOut - Time that the motor should run before stopping
     */
    void setWheelSpeeds(const float leftWheelSpeed, 
                        const float rightWheelSpeed,
                        const int timeOut) 
    {  
      leftWheel.motor.setSpeed(leftWheelSpeed, timeOut); 
      rightWheel.motor.setSpeed(rightWheelSpeed, timeOut); 
    }

    /**
     * Returns true if the motors are running
     * 
     * @return true/false - Depending on the state of the motors
     */
    bool active() {
      if (leftWheel.motor.isRunning() || rightWheel.motor.isRunning()) {
        return true;
      } else {
        return false;
      }      
    }

    /**
     * Returns true if the motors have stopped
     * 
     * @return true/false - Depending on the state of the motors
     */
    bool inActive() { return !active(); }

};

#endif // _DRIVE_TRAIN_H_