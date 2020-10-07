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

  // Display labels to the OLED
  clearDisplayBelowHeader();
  drawText(1, 0, "L");
  drawText(1, 50, "R");

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

  // Get the number of pulses from each encoder
  const int currentLeftPulses = robot_.driveTrain.leftWheel.motor.getPulses();
  const int currentRightPulses = robot_.driveTrain.rightWheel.motor.getPulses();
  const int leftPulses = currentLeftPulses - lastLeftPulses_;
  const int rightPulses = currentRightPulses - lastRightPulses_; 
  
  // Display pulses for this command to the OLED
  drawText(1, 10, String(leftPulses));
  drawText(1, 60, String(rightPulses));
}

// -------------------------------------------------------------
// Handles tasks to be performed after a command has completed.
// -------------------------------------------------------------  
void Command::end() {

  // Execute these tasks if the command is running
  if (commandRunning) { 

    // Turn off the LED
    digitalWrite(LED_BUILTIN, LOW);

    // Get the number of pulses from each encoder
    int leftPulsesPerSecond = robot_.driveTrain.leftWheel.motor.getPulsesPerSecond();
    int rightPulsesPerSecond = robot_.driveTrain.rightWheel.motor.getPulsesPerSecond();
    
    // Display pulses per second for this command to the OLED
    drawText(2, 0, "L");
    drawText(2, 10, String(leftPulsesPerSecond));
    drawText(2, 50, "R");
    drawText(2, 60, String(rightPulsesPerSecond));

    digitalWrite(LED_BUILTIN, LOW);

    // Now we're completely done and the command so no longer running
    commandRunning = false;
  }
}