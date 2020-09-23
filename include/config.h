#ifndef _CONFIG_H_
#define _CONFIG_H_

// -------------- Hardware interfaces ----------------

int LED_BUILTIN = 2;

//----------------------------------
// Include timer interrupt code
//----------------------------------
//  #include <esp_timer.h>
// inline esp_timer_handle_t stateUpdateTimer;
// inline esp_timer_handle_t motorTimer0, motorTimer1;

//---------------------------------------
// Define the GPIO pins for the motors
//---------------------------------------
static struct MotorPins {
  const byte motorDir1; // motor direction pin1
  const byte motorDir2; // motor direction pin2
  const byte enable; // Enable PMW 
} motorPinGroup[2] = {27, 26, 25, 
                      12, 14, 13};

// PMW for motors
const int freq = 30000;
const int resolution = 8;

//-------------------------------------------------//
// Function to return the sign of a value
//-------------------------------------------------//
template<typename T>
static constexpr int sgn(const T val) {   
 if (val < 0) return -1;
 if (val == 0) return 0;
 return 1;
}

// -------------------------------------------------------//
// OLED configuration                                     //
// -------------------------------------------------------//
#define USE_OLED true

#endif // _CONFIG_H_