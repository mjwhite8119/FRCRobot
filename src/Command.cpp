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
  maxLeftVelocity_ = 0;
  maxRightVelocity_ = 0;

  // Display labels to the OLED
  clearDisplayBelowHeader();
  drawText(1, 0, "L");
  drawText(1, 40, "R");
  drawText(1, 80, "Vel. p/s");

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

  // Get the linear velocity from each encoder
  float leftVelocityPerSecond = robot_.driveTrain.leftWheel.getVelocityPerSecond();
  float rightVelocityPerSecond = robot_.driveTrain.rightWheel.getVelocityPerSecond();

  // Display velocity per second to the OLED
  drawText(1, 10, String(leftVelocityPerSecond));
  drawText(1, 50, String(rightVelocityPerSecond));

  // Log the max velocity reached during this command
  if (leftVelocityPerSecond > maxLeftVelocity_) {
    maxLeftVelocity_ = leftVelocityPerSecond;
  }
  if (rightVelocityPerSecond > maxRightVelocity_) {
    maxRightVelocity_ = rightVelocityPerSecond;
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

    // Get the linear velocity from each encoder
    float leftVelocityPerSecond = robot_.driveTrain.leftWheel.getVelocityPerSecond();
    float rightVelocityPerSecond = robot_.driveTrain.rightWheel.getVelocityPerSecond();

    // Display final velocity per second to the OLED
    drawText(1, 10, String(leftVelocityPerSecond));
    drawText(1, 50, String(rightVelocityPerSecond));

    // // Display the max velocity for this command to the OLED
    drawText(2, 0, "L");
    drawText(2, 10, String(maxLeftVelocity_));
    drawText(2, 40, "R");
    drawText(2, 50, String(maxRightVelocity_));
    drawText(2, 80, "Max vel.");

    digitalWrite(LED_BUILTIN, LOW);

    // Now we're completely done and the command so no longer running
    commandRunning = false;
  }
}