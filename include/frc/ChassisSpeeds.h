/**
 * Represents the speed of a robot chassis w.r.t to the robot
 * frame of reference.
 *
 * A strictly non-holonomic drivetrain, such as a differential drive, should
 * never have a dy component because it can never move sideways. Holonomic
 * drivetrains such as swerve and mecanum will often have all three components.
 */
struct ChassisSpeeds {
  /**
   * Represents forward velocity w.r.t the robot frame of reference. (Fwd is +)
   */
  float vx = 0;

  /**
   * Represents strafe velocity w.r.t the robot frame of reference. (Left is +)
   */
  float vy = 0;

  /**
   * Represents the angular velocity of the robot frame. (CCW is +)
   */
  float omega = 0;

};