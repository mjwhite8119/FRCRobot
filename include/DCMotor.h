#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include "Encoder.h"
    
class DCMotor
{
  public:
        
    DCMotor() {} // Default constructor

    /**
     * Constructor. Connects motor GPIO pin to microcontroller
     * 
     * @param pinGroup - GPIO pin group for this motor.
     */
    explicit DCMotor(const uint8_t pinGroup);

    // Encoder attached to the motor
    Encoder encoder;

    const int maxPulsesPerSecond = 700; 

    // Include feed forward class
    const float kS = 200.0;
    const float kV = 50.0;

    // Mutex for protecting critical code in interrupts
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
     
    /**
     * This sets the speed of the motor.
     * Input is a speed ratio between -1.0 (backwards) and 1.0 (forward). 
     * It takes the requested wheel speed and calculates the number of
     * pulses per second required to achieve that speed.  
     * 
     * @param wheelSpeed - Speed ratio between -1.0 and 1.0.
     */
    void setSpeed(const float wheelSpeed);

    /**
     * Returns the current PWM value for this motor.
     */
    int getPWM() const {return PWM_;}

    /**
     * Returns the current pulses per second.
     */
    int32_t getPulses() const {return encoder.pulses;}

    /**
     * Returns the current pulses per second.
     */
    // int32_t getPulsesPerSecond() const {return pulsesPerSec_;}

    /**
     * Returns the current proportional part of the PID
     */
    // float getProportional() const {return pPart_;}

    /**
     * Returns the feedforward value. This is the minimum PWM value required
     * to maintain the motor velocity. For these motors it has to be at least
     * a value of 150 PWM.
     */
    // float getFeedforward() const {return (kS + (kV * wheelSpeedProportion_));}

    /**
     * Returns the current pulse setpoint. The pulse setpoint is 
     * calculated in the setSpeed() function.
     */
    // int getPulseSetpoint() const {return pulseSetpoint_;}

  private:
    
    uint8_t pinGroup_; // motor GPIO pins 

    static DCMotor * instances [2];

    /**
     * Timer interupt callback routines to call the setPower() function
     * every 25 milliseconds.  The timer interupt is setup in the constructor.
     */
    static void motorISR0 (void *pArg)
    {
      if (DCMotor::instances [0] != NULL)
        DCMotor::instances [0]->setPower_();
    } 
    
    static void motorISR1 (void *pArg)
    {
      if (DCMotor::instances [1] != NULL)
        DCMotor::instances [1]->setPower_();
    }
    
    // Motor speed variables
    const int feedForward = 180;
    int PWM_ = 0; // Current PWM

    // int32_t pulsesLast_ = 0; 
    // int32_t pulsesPerSec_ = 0;
    // int error_ = 0;      
    // float pPart_ = 0.0, iPart_ = 0.0; // PI control
    // int pulseSetpoint_ = 0; // Current pulse setpoint
    // double lastMessageTime_;
    // const int timeOut_ = 1000;
    // float wheelSpeedProportion_ = 0.0;
    
    // PI control. Adjust gain constants as necessary.  Place in RAM
    // const float Kp = 0.17, Ki = 0.1; // gain constants

    /**
     * Set motor power. This implements a PID loop which runs every 25ms.
     * It compares the difference between the current pulses per/sec and
     * the required pulses per/sec of each motor and adjusts the power
     * PWM signal to maintain the required speed of each wheel.
     */
    void IRAM_ATTR setPower_();

    /**
     * This applies the direction and power to the motor.
     * 
     * @param dir - Direction of motor
     * @param PWM - Power to the motor as a PWM signal
     */
    void IRAM_ATTR applyPower_(const int dir, const int PWM);
};
#endif // _DC_MOTOR_H_