#include <Arduino.h>
#include "config.h"
#include "OLED.h"
#include "connectWiFi.h"
#include "WebSite1.h"
#include "Robot.h"

// Create robot object
Robot robot;

void testSPIFFS() {
 
  File file2 = SPIFFS.open("/index.html", "r");
 
  if (!file2) {
    Serial.println("Failed to open file for reading");
    return;
  }
 
  Serial.println("File Content:");
 
  while (file2.available()) {
 
    Serial.write(file2.read());
  }
 
  file2.close();
}

// ----------------------------------------------------------
// Setup
// ----------------------------------------------------------
void setup() {

  Serial.begin(115200); while(!Serial && !Serial.available()){}

  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the OLED.
  #if USE_OLED
    setupOLED();
  #endif

  // Start the SPIFFS filesystem to serve the HTML pages
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wifi
  connectWiFi();

  // Setup for Over-the-Air updates
  setupOTA();

  // Define the web pages and listen for incoming client requests
  handleWebServer();

  // Begin the WebServer
  server.begin();

  // testSPIFFS();

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
      Serial.println("Forward");
      robot.forward(period, PWM);
    } else if (direction.compareTo("B") == 0) {
      Serial.println("Backward");
      robot.backward(period, PWM);
    } else if (direction.compareTo("L") == 0) {
      Serial.println("Left");
      robot.left(period, PWM);
    } else if (direction.compareTo("R") == 0) {
      Serial.println("Right");
      robot.right(period, PWM);
    }        
    period = 0;
  }
  

}