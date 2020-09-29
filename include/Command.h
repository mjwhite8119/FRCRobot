#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "Arduino.h"
#include "Robot.h"
#include "OLED.h"

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

    // Command specific variables
    String direction = ""; // Direction backward/forward to move the robot
    int period = 0; // How long to run the motors
    int leftPWM = 0; // Motor left PWM value
    int rightPWM = 0; // Motor right PWM value

    /**
     * Handles tasks to be performed when a command is scheduled.
     * The command gets scheduled from the WebSite.
     * This is called from the main loop.
     */ 
    void initialize();

    /**
     * Handles tasks to be performed while the command is running.
     * This is called from the main loop.
     */ 
    void execute();

    /**
     * Handles tasks to be performed after a command has completed.
     * This is called from the main loop.
     */ 
    void end();

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

    private:
      Robot & robot_;

      double lastLeftPulses_;
      double lastRightPulses_;

};

#endif // _COMMAND_H_