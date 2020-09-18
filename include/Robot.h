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
     * Move robot forward
     */ 
    void forward() {
      driveTrain.setWheelSpeeds(1, 1, 180, 178);
    }

    /**
     * Move robot backward
     */ 
    void backward() {
      driveTrain.setWheelSpeeds(-1, -1, 180, 179);
    }

    /**
     * Spin the robot to the left
     */ 
    void left() {
      driveTrain.setWheelSpeeds(-1, 1, 170, 170);
    }

    /**
     * Spin the robot to the right
     */ 
    void right() {
      driveTrain.setWheelSpeeds(1, -1, 170, 170);
    }

    /**
     * Stop the robot
     */ 
    void stop() {
      driveTrain.setWheelSpeeds(0, 0, 0 ,0);
    }
};

#endif _ROBOT_H_