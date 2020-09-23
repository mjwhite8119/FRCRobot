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
    void forward(const int period) {
      driveTrain.setWheelSpeeds(180, 178);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(period);
      stop();
    }

    /**
     * Move robot backward
     */ 
    void backward(const int period) {
      driveTrain.setWheelSpeeds(180, 179);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(period);
      stop();
    }

    /**
     * Spin the robot to the left
     */ 
    void left(const int period) {
      driveTrain.setWheelSpeeds(170, 170);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(period);
      stop();
    }

    /**
     * Spin the robot to the right
     */ 
    void right(const int period) {
      driveTrain.setWheelSpeeds(170, 170);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(period);
      stop();
    }

    /**
     * Stop the robot
     */ 
    void stop() {
      driveTrain.setWheelSpeeds(0, 0);
      digitalWrite(LED_BUILTIN, LOW);
    }
};

#endif _ROBOT_H_