#ifndef _DRIVE_TRAIN_H_
#define _DRIVE_TRAIN_H_

#include "Wheel.h"
#include "DifferentialDriveWheelSpeeds.h"
#include "ChassisSpeeds.h"
#include "IMU.h"

#include "frc/Rotation2d.h"

class DriveTrain
{
  public:
    
    // Constructor
    DriveTrain() {}

    const uint8_t leftWheelPinGroup = 0; // GPIO pin group. See config.h
    const uint8_t rightWheelPinGroup = 1; // GPIO pin group. See config.h
    
    Wheel leftWheel = Wheel(leftWheelPinGroup); // Attach the left wheel
    Wheel rightWheel = Wheel(rightWheelPinGroup); // Attach the right wheel

    float trackWidth = 0.140; // Distance between wheels in meters

    // IMU of the robot
    IMU imu;

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
     * Returns the left and right wheel speeds into the
     * structure DifferentialDriveWheelSpeeds
     * 
     * @return left and right wheel speeds
     */
    DifferentialDriveWheelSpeeds getWheelSpeeds () 
    {
      // Get the linear velocity from each wheel
      return {leftWheel.getVelocityPerSecond(),
              rightWheel.getVelocityPerSecond()};     
    }

    /**
     * Returns a chassis speed from left and right component velocities using
     * forward kinematics.
     *
     * @param wheelSpeeds The left and right velocities.
     * @return The chassis speed.
     */
    ChassisSpeeds toChassisSpeeds() 
    {
      DifferentialDriveWheelSpeeds wheelSpeeds = getWheelSpeeds();
      return {(wheelSpeeds.left + wheelSpeeds.right) / 2.0, 
              0,
              (wheelSpeeds.right - wheelSpeeds.left) / trackWidth * 1};
    }

    /**
     * Gets the robot heading from the IMU
     * 
     * @return Rotation2d object that includes the robot heading
     */
    Rotation2d getRobotRotation() {
      imu.update();
      float heading = imu.getRotationRadians();
      if (heading < 0.0) {heading += TWO_PI;}
      return Rotation2d(heading); 
    };

    /**
     * Set the initial heading of the robot
     *
     */
    void setGyroOffset() {
      Rotation2d offset = getRobotRotation();
      log_d("Set Offset... %2.0f", offset.Degrees());

      gyroOffset_ += offset;
      log_d("Set Offset... %2.0f", gyroOffset_.Degrees());
    }

    /**
     * Gets the offset of the Gryo
     * 
     * @return Rotation2d object that includes the gyro offset
     */
    Rotation2d getGyroOffset() { return gyroOffset_; }  

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

  private:
    Rotation2d gyroOffset_; // The initial offset of the robot heading
};

#endif // _DRIVE_TRAIN_H_