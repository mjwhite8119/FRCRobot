#include "Command.h"

// -------------------------------------------------------------
// Handles tasks to be performed when a command is scheduled.
// -------------------------------------------------------------  
void Command::initialize() {

  // Start the motors
  
  Serial.print("Move ");
  robot_.move(period, leftSpeed, rightSpeed);
  
  // The command is now running and not finished
  commandRunning = true;
  commandFinished = false;

  // Command is now running so no longer need to schedule
  commandSchedule = false; 

  // Save the number of pulses so far
  lastLeftPulses_ = robot_.driveTrain.leftWheel.motor.getPulses();
  lastRightPulses_ = robot_.driveTrain.rightWheel.motor.getPulses();

  // Used to keep track of the max velocity reached during current command
  maxVx_ = 0;
  maxOmega_ = 0;

  // Display labels to the OLED
  clearDisplayBelowHeader();
  drawText(1, 0, "Vx");
  drawText(1, 50, "Omega");

  digitalWrite(LED_BUILTIN, HIGH);
}

// -----------------------------------------------------------
// Handles tasks to be performed while the command is running.
// ----------------------------------------------------------- 
void Command::execute() {

  // If the driveTrain is inactive then the command is finished
  if (robot_.driveTrain.inActive()) {
    commandFinished = true;
  }

  // Get the linear and angular velocity of the Chassis
  ChassisSpeeds chassisSpeed = robot_.driveTrain.toChassisSpeeds();

  // Convert omega to whole degrees
  int omega = (int)(chassisSpeed.omega * RAD_TO_DEG);

  // Display velocity per second to the OLED
  drawText(1, 15, String(chassisSpeed.vx));
  drawText(1, 85, String(omega));

  // Log the max velocity reached during this command
  if (chassisSpeed.vx > maxVx_) {
    maxVx_ = chassisSpeed.vx;
  }
  if (chassisSpeed.omega > maxOmega_) {
    maxOmega_ = chassisSpeed.omega;
  }

}

// -------------------------------------------------------------
// Handles tasks to be performed after a command has completed.
// -------------------------------------------------------------  
void Command::end() {

  // Execute these tasks if the command is running
  if (commandRunning) { 

    // Turn off the LED
    digitalWrite(LED_BUILTIN, LOW);

    // Get the linear and angular velocity of the Chassis
    ChassisSpeeds chassisSpeed = robot_.driveTrain.toChassisSpeeds();

    // Convert omega to whole degrees
    int omega = (int)(chassisSpeed.omega * RAD_TO_DEG);

    // Display final velocity per second to the OLED
    drawText(1, 15, String(chassisSpeed.vx));
    drawText(1, 85, String(omega));

    // Display the max velocity for this command to the OLED
    // Convert omega to whole degrees
    int maxOmega = (int)(maxOmega_ * RAD_TO_DEG);

    drawText(2, 15, String(maxVx_));
    drawText(2, 85, String(maxOmega));
    drawText(2, 110, "Max");

    digitalWrite(LED_BUILTIN, LOW);

    // Now we're completely done and the command so no longer running
    commandRunning = false;
  }
}