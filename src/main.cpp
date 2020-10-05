#include <Arduino.h>
#include "config.h"
#include "OLED.h"
#include "connectWiFi.h"
#include "Command.h"
#include "Robot.h"
// #include "Controller.h"

// Create the Robot object
Robot robot;

// Create the Command object
Command command(robot);

// Create the Controller object
// Controller controller(command);

#include "WebSite.h"

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

  // Setup the controller webpage
  // controller.init();
  handleWebServer();

}

// ----------------------------------------------------------
// Main loop
// ----------------------------------------------------------
void loop() {

  // Must include to handle OTA updates
  ArduinoOTA.handle();

  // Initialize commands received from the website
  if (command.isScheduled()) {
    command.initialize();
  }  

  // Handle the running command
  if (command.isRunning()) {
    command.execute();
  }

  // Run tasks after command is finished
  if (command.isFinished()) {
    command.end();
  }

}