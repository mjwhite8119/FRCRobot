#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include "config.h"
    
class DCMotor
{
  public:
        
    DCMotor() {} // Default constructor

    // Constructor to connect motor GPIO pins to microcontroller
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

    void applyPower(const int dir, const int PWM) {
      int level;
      if(dir >= 0) {
        level = HIGH; 
      } else {
        level = LOW; 
      }

      digitalWrite(motorPinGroup[pinGroup_].motorDir2, level); // Direction HIGH forward, LOW backward
      digitalWrite(motorPinGroup[pinGroup_].motorDir1, (!level)); // Write the opposite value
      
      // See pinGroup in Config.h
      ledcWrite(pinGroup_, abs(PWM));
    }

  private:
    
    uint8_t pinGroup_; // motor GPIO pins   
};

#endif // _DC_MOTOR_H_