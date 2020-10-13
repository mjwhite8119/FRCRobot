#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include "config.h"
    
class DCMotor
{
  public:
        
    DCMotor() {} // Default constructor

    /**
     * Constructor. Connects motor GPIO pins to microcontroller
     *
     * @param pinGroup  GPIO pin group for this motor.
     */
    explicit DCMotor(const uint8_t pinGroup)
        :pinGroup_(pinGroup)
    {   
      // Connect motor to GPIO pins
      pinMode(motorPinGroup[pinGroup].motorDir1, OUTPUT); // motor direction
      pinMode(motorPinGroup[pinGroup].motorDir2, OUTPUT); // motor direction
      pinMode(motorPinGroup[pinGroup].enable, OUTPUT);

      // Setup PWM signal
      ledcSetup(pinGroup_, freq, resolution); // create a PWM channel 
      ledcAttachPin(motorPinGroup[pinGroup_].enable, pinGroup_); // attach channel to pin;
    }

    /**
     * Apply power to the motor
     * 
     * @param dir Direction of motor.
     * @param PWM The PWM value to send to the motor.
     */
    void applyPower(const int dir, const int PWM) {
      int level;
      if(dir >= 0) {
        level = HIGH; 
      } else {
        level = LOW; 
      }

      digitalWrite(motorPinGroup[pinGroup_].motorDir2, level); // Direction HIGH forward, LOW backward
      digitalWrite(motorPinGroup[pinGroup_].motorDir1, (!level)); // Write the opposite value
      
      // Power the motor. See pinGroup in Config.h
      ledcWrite(pinGroup_, abs(PWM));

      // Flag the running status of the motor
      if (PWM > 0) {
        running_ = true;
      } else {
        running_ = false;
      }
    }

    /**
     * Is the motor currently running
     * 
     * @return true/false - The running status of the motor
     */
    bool isRunning() { return running_; }

  private:
    
    // Variable added in lesson 1
    uint8_t pinGroup_; // motor GPIO pins  
    bool running_ = false; // Is the motor currently running 
};

#endif // _DC_MOTOR_H_