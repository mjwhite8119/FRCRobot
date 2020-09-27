#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "Arduino.h"
#include "config.h"

// Temporary variable to simulate encoder interrupt
const int interruptFreq = 10; // Every 10 milliseconds 
const int interruptsPerSecond = 1000 / interruptFreq;

class Encoder
{  
   public:

    // Default constructor
    Encoder() {}

    /**
     * Constructor. Connects encoder GPIO pin to microcontroller
     * 
     * @param pinGroup - GPIO pin group for this encoder.
     */
    explicit Encoder(uint8_t pinGroup);

    // Class variables
    static constexpr int PPR = 20;  // Encoder Count per Revolutions 

    // Mutex for protecting critical code in interrupts
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

    volatile int32_t pulses;
    int8_t wheelDirection = 0;

    /**
     * Reset pulses.
     */
    void resetEncoder() {pulses = 0;}

    void setWheelDirection(const int dir) { wheelDirection = dir; }

    static Encoder * instances [2];
   
  private:

    uint8_t pinGroup_;
 
    /**
     * Encoder interrupt routines. These fire from an external interupt
     * GPIO pin that connects to the encoder.
     */
    static void encoderISR0 ()
    {
      if (Encoder::instances [0] != NULL)
        Encoder::instances [0]->encoderFired_();
    } 
    
    static void encoderISR1 ()
    {
      if (Encoder::instances [1] != NULL)
        Encoder::instances [1]->encoderFired_();
    }

    /**
     * This routine runs each time an interrupt fires.  It increases
     * or decreases the number of pulses.
     */
    void IRAM_ATTR encoderFired_();
    
};
#endif // _ENCODER_H_