#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "DriveTrain.h"

class Robot
{
  public:

    // --- Constructor ---
    Robot() {}

    // --- Member classes (Robot configuration) ---

    // Drive train of robot
    DriveTrain driveTrain;

    /**
     * Move robot 
     * 
     * @param period - Period to run the motor in seconds
     * @param leftSpeed - Left wheel speed between -1 and +1
     * @param rightSpeed - Right wheel speed between -1 and +1
     */ 
    void move(const float leftSpeed, const float rightSpeed) {

      // Also send the max timeout of 10 seconds
      driveTrain.setWheelSpeeds(leftSpeed, rightSpeed, maxTimeOut);
    }

    /**
     * Stop robot 
     */ 
    void stop() {
      driveTrain.setWheelSpeeds(0.0, 0.0, 0);
    }


};

#endif // _ROBOT_H_