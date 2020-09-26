#include <Arduino.h>
#include "config.h"
#include "OLED.h"
#include "connectWiFi.h"
#include "WebSite.h"
#include "Robot.h"

// Create robot object
Robot robot;

// ----------------------------------------------------------
// Setup
// ----------------------------------------------------------
void setup() {

  // Setup to write to the serial console for debugging
  Serial.begin(115200); while(!Serial && !Serial.available()){}

  // This is the GPIO pin of the LED that is on the ESP32
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the OLED.
  #if USE_OLED
    setupOLED();
  #endif

  // Connect to Wifi
  connectWiFi();

  // Setup for Over-the-Air updates
  setupOTA();

  // Setup the web pages and listen for incoming web client requests
  handleWebServer();

}

// ----------------------------------------------------------
// Main loop
// ----------------------------------------------------------
void loop() {

  // Must include to handle OTA updates
  ArduinoOTA.handle();

  // Handle requests from the website
  if (period > 0) {
    if (direction.compareTo("F") == 0) {
      Serial.print("Forward ");
      Serial.print(leftPWM);Serial.print(" ");Serial.println(rightPWM);
      robot.forward(period, leftPWM, rightPWM);
    } else if (direction.compareTo("B") == 0) {
      Serial.println("Backward ");
      Serial.print(leftPWM);Serial.print(" ");Serial.println(rightPWM);
      robot.backward(period, leftPWM, rightPWM);
    } 
    period = 0; // Reset period
  }
  

}