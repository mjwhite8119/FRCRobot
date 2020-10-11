#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include "Encoder.h"

//----------------------------------
// Include timer interrupt code
//----------------------------------
inline esp_timer_handle_t stateUpdateTimer;
inline esp_timer_handle_t motorTimer0, motorTimer1;
    
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

    // const int maxPulsesPerSecond = 700; 

    // Mutex for protecting critical code in interrupts
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

    /**
     * This sets the speed of the motor.
     * 
     * @param wheelSpeed - wheel speed value between -1 and +1
     * @param timeOut - Time that the motor should run before stopping
     */
    void setSpeed(const float wheelSpeed, 
                  const int timeOut);              

    /**
     * Is the motor currently running
     * 
     * @return true/false - The running status of the motor
     */
    bool isRunning() { return running_; }

    /**
     * Returns the current PWM value for this motor.
     */
    int getPWM() const {return PWM_;}

    /**
     * Returns the current total number pulses.
     */
    int32_t getPulses() const {return encoder.pulses;}

    /**
     * Returns the number of pulses per second.
     */
    int32_t getPulsesPerSecond() const {return pulsesPerSec_;}

  private:
    
    // Motor speed variables
    int PWM_ = 0; // Current PWM
    int direction_ = 0; // Direction of the motor
    int timeOut_ = 0; // Length of time motor runs
    unsigned long currentStartTime_ = 0; // Start time for this request
    int32_t pulsesLast_ = 0; 
    int32_t pulsesPerSec_ = 0;
    bool running_ = false; // Current running status of the motor

    int kStatic_ = 180; // Minimum PWM required to get the motor moving
    int maxPWM_ = 250; // Maximum  PWM value
    
    /**
     * Set motor power. This function is run every 25ms from the timer interrupt
     * It checks to see if the timeout period has expired in which case it stops
     * the motor.  Otherwise, is sends the PWM signal to the motor.
     */
    void IRAM_ATTR setPower_();

    /**
     * This applies the direction and power to the motor.
     * 
     * @param dir - Direction of motor
     * @param PWM - Power to the motor as a PWM signal
     */
    void IRAM_ATTR applyPower_(const int dir, const int PWM);

    uint8_t pinGroup_; // motor GPIO pins 

    static DCMotor * instances [2];

    /**
     * Timer interupt callback routine calls the setPower() function
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

};
#endif // _DC_MOTOR_H_