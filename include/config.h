#ifndef _CONFIG_H_
#define _CONFIG_H_

// -------------- Hardware interfaces ----------------
const int LED_BUILTIN = 2;

//---------------------------------------
// Define the GPIO pins for the motors
//---------------------------------------
static struct MotorPins {
  const byte motorDir1; // motor direction pin1
  const byte motorDir2; // motor direction pin2
  const byte enable; // Enable PMW 
  const byte encoder; // Wheel encoder
} motorPinGroup[2] = {12, 14, 13, 34,
                      27, 26, 25, 35};

// PMW for motors
const int freq = 30000;
const int resolution = 8;

const int maxTimeOut = 10*1000; // Stops the robot after timeout seconds

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