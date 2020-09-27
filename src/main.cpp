#include <Arduino.h>
#include "config.h"
#include "OLED.h"
#include "connectWiFi.h"

// Command flags. Must be placed before including WebSite
bool commandRunning = false;
bool commandScheduled = false;

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

  // Handle commands from the controller
  if (commandScheduled) {
    if (direction.compareTo("F") == 0) {
      Serial.print("Forward ");
      robot.forward(period, leftPWM, rightPWM);
    } else if (direction.compareTo("B") == 0) {
      Serial.println("Backward ");
      robot.backward(period, leftPWM, rightPWM);
    } 
    // The command is now running
    commandRunning = true;

    // Reset to false so as this code section only runs once
    commandScheduled = false; 
  }

  // Handle the running command
  robot.handleRunningCommand();

  // Run tasks after command is finished
  robot.handleEndCommand();
  
}