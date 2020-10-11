#include "Encoder.h"

// ------------------- Initialize static instances --------------

Encoder * Encoder::instances [2] = { NULL, NULL };

// ---------------------  Constructors -------------------------------

Encoder::Encoder(uint8_t pinGroup) 
  :pinGroup_(pinGroup)
{   
  // Connect encoder to GPIO pins
  pinMode(motorPinGroup[pinGroup].encoder, INPUT_PULLUP); //  Encoder pin
  // digitalWrite(motorPinGroup[pinGroup].encoder, HIGH); // turn on pullup resistors

  // Attach interrupts
  switch (pinGroup)
  {
  case 0: 
    attachInterrupt (motorPinGroup[0].encoder, encoderISR0, RISING);  // Left encoder
    instances [0] = this; 
    break;
    
  case 1: 
    attachInterrupt (motorPinGroup[1].encoder, encoderISR1, RISING); // Right encoder
    instances [1] = this;
    break;
    
  } // end of switch

  // Switch on interrupts
  sei();
  
  // Initialize pulses. 
  pulses = 0;

}

// ----------------  Private member methods ------------------------------
void IRAM_ATTR Encoder::encoderFired_() {
  // pulses is 4 bytes so make sure that the write is not interupted
  portENTER_CRITICAL_ISR(&timerMux);
  
  if (wheelDirection > 0) {
    pulses++;
  } else {
    pulses--;  
  } 
  portEXIT_CRITICAL_ISR(&timerMux);
}