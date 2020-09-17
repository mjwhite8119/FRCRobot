#include <Arduino.h>
#include "connectWiFi.h"
#include "WebServer.h"
#include "Robot.h"

// Create robot object
Robot robot;

// ----------------------------------------------------------
// Setup
// ----------------------------------------------------------
void setup() {

  Serial.begin(115200); while(!Serial && !Serial.available()){}
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to Wifi
  connectWiFi();

  // Setup for Over-the-Air uploads
  setupOTA();

  // Begin the WebServer
  server.begin();
}

// ----------------------------------------------------------
// Main loop
// ----------------------------------------------------------
void loop() {

  // Must include to handle OTA updates
  ArduinoOTA.handle();

  // Listen for incoming client requests to the Web Server 
  handleWebServer(robot);

}