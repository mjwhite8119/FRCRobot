#include "Command.h"

// -------------------------------------------------------------
// Handles tasks to be performed when a command is scheduled.
// -------------------------------------------------------------  
void Command::initialize() {

  // Start the motors
  if (direction.compareTo("F") == 0) {
    Serial.print("Forward ");
    robot_.forward(period);
  } else if (direction.compareTo("B") == 0) {
    Serial.println("Backward ");
    robot_.backward(period);
  } else if (direction.compareTo("L") == 0) {
    Serial.println("Left");
    robot_.left(period);
  } else if (direction.compareTo("R") == 0) {
    Serial.println("Right");
    robot_.right(period);
  }    

  // The command is now running and not finished
  commandRunning = true;
  commandFinished = false;

  // Command is now running so no longer need to schedule
  commandSchedule = false; 

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