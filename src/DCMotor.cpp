#include "DCMotor.h"

// ------------------- Initialize static instances --------------

DCMotor * DCMotor::instances[2] = { NULL, NULL };

// Frequency at which motor power is monitored
const int speedCtrlPeriodMillis = 100;             // speed control period in milli seconds
const int speedCtrlPeriodMicros = speedCtrlPeriodMillis*1000; // speed control period in micro seconds
const int DRAM_ATTR periodsPerSec = 1000/speedCtrlPeriodMillis;    // number of speed control periods/sec

// -------------------Constructors -----------------------------------

DCMotor::DCMotor(const uint8_t pinGroup) 
  :encoder(pinGroup), pinGroup_(pinGroup)
{   
  // Connect motor to GPIO pins
  pinMode(motorPinGroup[pinGroup].motorDir1, OUTPUT); // motor direction
  pinMode(motorPinGroup[pinGroup].motorDir2, OUTPUT); // motor direction
  pinMode(motorPinGroup[pinGroup].enable, OUTPUT);

  // Setup PWM signal
  ledcSetup(pinGroup_, freq, resolution); // create a PWM channel 
  ledcAttachPin(motorPinGroup[pinGroup_].enable, pinGroup_); // attach channel to pin

  // Start motor power timers 
  switch (pinGroup)
  {
  case 0: 
    {
      const esp_timer_create_args_t periodic_timer_args = {.callback = &motorISR0};
      esp_timer_create(&periodic_timer_args, &motorTimer0);
      esp_timer_start_periodic(motorTimer0, speedCtrlPeriodMicros); // Time in milliseconds (100)
      instances [0] = this; 
    }
    break;
    
  case 1: 
    {
      const esp_timer_create_args_t periodic_timer_args = {.callback = &motorISR1};
      esp_timer_create(&periodic_timer_args, &motorTimer1);
      esp_timer_start_periodic(motorTimer1, speedCtrlPeriodMicros); // Time in milliseconds (100)
      instances [1] = this;
    }
    break;
    
  } // end of switch

}

// ---------------- Public member methods -------------------------

// -------------------------------------------------------- 
// Set the wheel speeds 
// --------------------------------------------------------
void DCMotor::setSpeed(const float wheelSpeed, 
                       const int timeOut) {

  // Save the pulses at the start of this request                     
  startingPulses_ = encoder.getPulses(); 
  lastPulses_ = encoder.getPulses(); 

  // Set the timeout to stop the motor
  timeOut_ = timeOut;
  currentStartTime_ = millis();

  // Compute feed forward PWM
  // kStatic is the minimum PWM value required to move the wheel. 
  // kVelocity is the additional PWM required to maintain the 
  // requested velocity.
  feedForwardPWM_ = kStaticPWM_ + (kVelocityPWM_ * abs(wheelSpeed));

  // Set the PWM value to the feedForward
  PWM_ = feedForwardPWM_;

  // Let the encoder know which direction it's spinning
  direction_ = sgn(wheelSpeed); 
  encoder.setWheelDirection(direction_);

  log_d("wheelspeed = %d", wheelSpeed);
  log_d("PWM = %d", PWM_);
  log_d("direction = %d", direction_);
  log_d("TimeOut = %d", timeOut);

}  

// ----------------------------------------------------------------
// Set motor power.  Runs 10 times per second
// ---------------------------------------------------------------- 
void IRAM_ATTR DCMotor::setPower_() {

  portENTER_CRITICAL_ISR(&timerMux);

  // Stop the motor after the timeout period
  if ( (millis() - currentStartTime_) > timeOut_ ) 
  {
    PWM_ = 0;

    // Compute pulses per second for this last motion request
    if (timeOut_ > 0) {
      const int32_t pulsesThisPeriod = abs(encoder.getPulses() - startingPulses_);
      avgPulsesPerSec_ = pulsesThisPeriod / (timeOut_ / 1000); // timeOut is in milliseconds
      // Reset the timeout
      timeOut_ = 0;
    }
  }  
  
  // Apply the power with the direction and PWM signal
  applyPower_(direction_, PWM_);

  // Calculate pulses per second
  int32_t pulses = encoder.getPulses();
  currentPulsesPerSec_ = abs(pulses - lastPulses_) * periodsPerSec;
  lastPulses_ = pulses;
  
  portEXIT_CRITICAL_ISR(&timerMux);
}

// ------------------------------------------------
// Apply power to motor 
// ------------------------------------------------
void IRAM_ATTR DCMotor::applyPower_(const int dir, const int PWM) {

  int level;
  if(dir >= 0) {
    level = HIGH; 
  } else {
    level = LOW; 
  }

  digitalWrite(motorPinGroup[pinGroup_].motorDir2, level); // Direction HIGH forward, LOW backward
  digitalWrite(motorPinGroup[pinGroup_].motorDir1, (!level)); // Write the opposite value
  
  // Send the PWM signal to the motor. See pinGroup in Config.h
  ledcWrite(pinGroup_, abs(PWM));
}