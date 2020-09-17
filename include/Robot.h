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

    void forward() {
      driveTrain.setWheelSpeeds(1, 1, 200, 200);
    }

    void backward() {
      driveTrain.setWheelSpeeds(-1, -1, 200, 200);
    }

    void stop() {
      driveTrain.setWheelSpeeds(0, 0, 0 ,0);
    }
};

#endif _ROBOT_H_