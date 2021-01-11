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

  // Initialize the IMU and wait for it to settle
  robot.driveTrain.imu.initMPU6050();
  clearDisplayBelowHeader();
  for (int i=9; i > 0; i--) {
    drawText(2, 0, "Settling gyro");
    drawText(2, 90, String(i));
    delay(1000);
  } 

  // Set the gyro offset
  robot.driveTrain.setGyroOffset();
  Rotation2d offset = robot.driveTrain.getGyroOffset();
  log_d("offset = %2.1f", offset.Degrees());
 
  // Initialize the odometry
  Pose2d initialPose = Pose2d{Translation2d{0.0,0.0},Rotation2d{0.0}};
  robot.driveTrain.odometry.initialize(initialPose, offset);

  // Update the odometry
  robot.driveTrain.updateOdometry();

   // Display the labels
  clearDisplayBelowHeader();
  drawText(1, 0, "X");
  drawText(1, 50, "Y");
  drawText(2, 0, "Heading");
  drawText(2, 70, "degrees");

  // Display current Pose to the OLED
  Pose2d pose = robot.driveTrain.odometry.getPose();

  drawText(1, 10, String(pose.Translation().X()));
  drawText(1, 60, String(pose.Translation().Y()));
  drawText(2, 50, String((int)pose.Rotation().Degrees()));

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