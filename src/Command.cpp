#include "Command.h"

// -------------------------------------------------------------
// Handles tasks to be performed when a command is scheduled.
// -------------------------------------------------------------  
void Command::initialize() {

  // Start the motors
  robot_.move(period, leftSpeed, rightSpeed);
  
  // The command is now running and not finished
  commandRunning = true;
  commandFinished = false;

  // Command is now running so no longer need to schedule
  commandSchedule = false; 

  // Display labels to the OLED
  // clearDisplayBelowHeader();
  drawText(1, 0, "Heading");
  drawText(1, 70, "degrees");

  digitalWrite(LED_BUILTIN, HIGH);
}

// -----------------------------------------------------------
// Handles tasks to be performed while the command is running.
// This is called from the main loop.
// ----------------------------------------------------------- 
void Command::execute() {

  // If the driveTrain is inactive then the command is finished
  if (robot_.driveTrain.inActive()) {
    commandFinished = true;
  }

  // Get the gyro rotation angle from the IMU
  Rotation2d heading = robot_.driveTrain.getRobotRotation(); 
  log_d("yaw %2.0f", heading.Degrees());

  Rotation2d offset = robot_.driveTrain.getGyroOffset();
  log_d("offset %2.0f", offset.Degrees());

  // Subtract the gyro offset
  heading -= robot_.driveTrain.getGyroOffset();
  log_d("yaw - offset %2.0f", heading.Degrees());

  drawText(1, 50, String((int)heading.Degrees()));

}

// -------------------------------------------------------------
// Handles tasks to be performed after a command has completed.
// -------------------------------------------------------------  
void Command::end() {

  // Execute these tasks if the command is running
  if (commandRunning) { 

    // Turn off the LED
    digitalWrite(LED_BUILTIN, LOW);

    // Now we're completely done and the command so no longer running
    commandRunning = false;
  }
}