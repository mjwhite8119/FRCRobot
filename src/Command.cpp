#include "Command.h"

// -------------------------------------------------------------
// Handles tasks to be performed when a command is scheduled.
// -------------------------------------------------------------  
void Command::initialize() {

  // Start the motors
  robot_.move(leftSpeed, rightSpeed);
  
  // The command is now running and not finished
  commandRunning = true;
  commandFinished = false;

  // Command is now running so no longer need to schedule
  commandSchedule = false; 

  // Set the ending distance.  Current distance + requested distance
  endingDistance = robot_.driveTrain.getChassisDistance() + distance;
  log_d("ending distance = %2.1f", endingDistance);

  digitalWrite(LED_BUILTIN, HIGH);
}

// -----------------------------------------------------------
// Handles tasks to be performed while the command is running.
// This is called from the main loop.
// ----------------------------------------------------------- 
void Command::execute() {

  // Update the odometry
  robot_.driveTrain.updateOdometry();

  // Keep track of the distance travelled
  log_d("d2 = %2.1f", robot_.driveTrain.getChassisDistance());

  if (robot_.driveTrain.getChassisDistance() > endingDistance) {  
    robot_.stop();
    commandFinished = true;
  }

  // Display current Pose to the OLED
  Pose2d pose = robot_.driveTrain.odometry.getPose();

  drawText(1, 10, String(pose.Translation().X()));
  drawText(1, 60, String(pose.Translation().Y()));
  drawText(2, 50, String((int)pose.Rotation().Degrees()));

  log_d("x = %2.1f", pose.Translation().X());
  log_d("y = %2.1f", pose.Translation().Y());
  log_d("heading = %2.1f", pose.Rotation().Degrees());

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