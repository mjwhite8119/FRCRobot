#include <Arduino.h>
#include "config.h"
#include "OLED.h"
#include "connectWiFi.h"
#include "Command.h"
#include "Robot.h"
#include <Wire.h>
#include "Controller.h"

// Create the Robot object
Robot robot;

// Create the Command object
Command command(robot);

// ----------------------------------------------------------
// Scan the I2C bus for devices
// ----------------------------------------------------------
void i2cScan() {
  byte error, address; //variable for error and I2C address
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}

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

  // Diagnostic test for I2C connectivity
  i2cScan();

  // Setup the controller webpage
  setupController(command);

  // Initialize the IMU
  robot.driveTrain.imu.initMPU6050();

  // Get the offset of the gyro.  The gyro takes a few
  // seconds to settle. So wait a little.
  clearDisplayBelowHeader();
  for (int i=10; i > 0; i--) {
    drawText(2, 0, "Settling gyro");
    drawText(2, 90, String(i));
    delay(1000);
  } 
  robot.driveTrain.setGyroOffset();

  // Display the gyro offset
  clearDisplayBelowHeader();
  drawText(2, 0, "Gyro offset");
  float offset = robot.driveTrain.getGyroOffset().Degrees();
  drawText(2, 70, String((int)offset));

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