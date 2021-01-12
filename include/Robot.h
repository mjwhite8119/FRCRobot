#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "DriveTrain.h"

class Robot
{
  public:

    // --- Constructor ---
    Robot() {}

    // Drive train of robot
    DriveTrain driveTrain;

    /**
     * Move robot forward
     * 
     * @param period - Period to run the motor in seconds
     * @param leftPWM - Left wheel PWM signal
     * @param rightPWM - Right wheel PWM signal
     */ 
    void forward(const int period, const int leftPWM, const int rightPWM) {

      // Convert seconds to micro seconds since that's what the timer interrupt wants
      int timeOut = period*1000;     
      driveTrain.setWheelSpeeds(1, 1, leftPWM, rightPWM, timeOut);
    }

    /**
     * Move robot backward
     * 
     * @param period - Period to run the motor in seconds
     * @param leftPWM - Left wheel PWM signal
     * @param rightPWM - Right wheel PWM signal
     */ 
    void backward(const int period, const int leftPWM, const int rightPWM) {

      // Convert seconds to micro seconds since that's what the timer interrupt wants
      int timeOut = period*1000;
      driveTrain.setWheelSpeeds(-1, -1, leftPWM, rightPWM, timeOut);
    }

    /**
     * Stop the robot
     */ 
    void stop() {
      driveTrain.setWheelSpeeds(1, 1, 0, 0, 0);
      digitalWrite(LED_BUILTIN, LOW);
    }
};

#endif // _ROBOT_H_