#ifndef _CONFIG_H_
#define _CONFIG_H_

// -------------- Communcation interfaces ----------------

//------------------------------------
// WiFi Definitions GET THIS INTO SEPARATE FILE
//------------------------------------


// -------------- Hardware interfaces ----------------

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

// -------------------------------------------------------//
// OLED configuration                                     //
// -------------------------------------------------------//
#define USE_OLED true
#define HELTEC_WIFI_KIT_32 false
#define NODEMCU_32S true

#if USE_OLED
  // Library used for OLED
  #if (HELTEC_WIFI_KIT_32 || TTGO_LORA32_OLED)
    #define HELTEC true
    #define USE_U8G2 false
  #else  
    #define USE_U8G2 true
    #define HELTEC false
  #endif   
#endif

#endif // _CONFIG_H_