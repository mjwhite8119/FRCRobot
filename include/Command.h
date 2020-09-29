#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "Arduino.h"
#include "Robot.h"

class Command
{
  public:

    /**
     * Constructor
     * 
     * @param robot - The robot object on which the command operates
     */ 
    Command(Robot & robot) 
      :robot_(robot) {}

    // --- Member variables ---
    bool commandRunning = false;
    bool commandSchedule = false;
    bool commandFinished = true;

    // Robot & robot; 

    // Command specific variables
    String direction = ""; // Direction backward/forward to move the robot
    int period = 0; // How long to run the motors
    int leftPWM = 0; // Motor left PWM value
    int rightPWM = 0; // Motor right PWM value
    double lastLeftPulses;
    double lastRightPulses;

    /**
     * Schedule a new command.  This is called from the WebSite.
     */ 
    void schedule() { commandSchedule = true; }

    /**
     * Unschedule a new command.  This is called from the WebSite.
     */ 
    void unSchedule() { commandSchedule = false; }

    /**
     * Is the command scheduled?
     */ 
    bool isScheduled() { return commandSchedule; }

    /**
     * Is the command running?
     */ 
    bool isRunning() { return commandRunning; }

    /**
     * Is the command finished?
     */ 
    bool isFinished() { return commandFinished; }

    /**
     * Handles tasks to be performed when a command is scheduled.
     * The command gets scheduled from the WebSite.
     * This is called from the main loop.
     */ 
    void initialize() {

      // Start the motors
      if (direction.compareTo("F") == 0) {
        Serial.print("Forward ");
        robot_.forward(period, leftPWM, rightPWM);
      } else if (direction.compareTo("B") == 0) {
        Serial.println("Backward ");
        robot_.backward(period, leftPWM, rightPWM);
      } 

      // The command is now running and not finished
      commandRunning = true;
      commandFinished = false;

      // Command is now running so no longer need to schedule
      commandSchedule = false; 

      // Save the number of pulses so far
      // int lastLeftPulses = robot_.driveTrain.leftWheel.motor.getPulses();
      // int lastRightPulses = robot_.driveTrain.rightWheel.motor.getPulses();

      // Display labels to the OLED
      clearDisplayBelowHeader();
      drawText(1, 0, "L");
      drawText(1, 50, "R");

      digitalWrite(LED_BUILTIN, HIGH);
    }

    /**
     * Handles tasks to be performed while the command is running.
     * This is called from the main loop.
     */ 
    void execute() {

      // If the driveTrain is inactive then the command is finished
      if (robot_.driveTrain.inActive()) {
        commandFinished = true;
      }

      // Get the number of pulses from each encoder
      const int currentLeftPulses = robot_.driveTrain.leftWheel.motor.getPulses();
      const int currentRightPulses = robot_.driveTrain.rightWheel.motor.getPulses();
      const int leftPulses = currentLeftPulses - lastLeftPulses;
      const int rightPulses = currentRightPulses - lastRightPulses; 
      
      // Display pulses for this command to the OLED
      drawText(1, 10, String(leftPulses));
      drawText(1, 60, String(rightPulses));
    }

    /**
     * Handles tasks to be performed after a command has completed.
     * This is called from the main loop.
     */ 
    void end() {

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

    private:
      Robot & robot_;

};

#endif // _COMMAND_H_