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
    void move(const int period, const float leftSpeed, const float rightSpeed) {

      // Convert seconds to micro seconds since that's what the timer interrupt wants
      int timeOut = period*1000;     
      driveTrain.setWheelSpeeds(leftSpeed, rightSpeed, timeOut);
    }

};

#endif // _ROBOT_H_