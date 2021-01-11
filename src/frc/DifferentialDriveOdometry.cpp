/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/DifferentialDriveOdometry.h"

DifferentialDriveOdometry::DifferentialDriveOdometry(
    const Rotation2d& gyroAngle, const Pose2d& initialPose)
    : m_pose(initialPose) {
  m_previousAngle = m_pose.Rotation();
  m_gyroOffset = m_pose.Rotation() - gyroAngle;
}

const Pose2d& DifferentialDriveOdometry::update(const Rotation2d& gyroAngle,
                                                float leftDistance,
                                                float rightDistance) {
                                                
  // Step 1 - compute the delta distance since the last time step
  auto deltaLeftDistance = leftDistance - m_prevLeftDistance;
  auto deltaRightDistance = rightDistance - m_prevRightDistance;

  m_prevLeftDistance = leftDistance;
  m_prevRightDistance = rightDistance;
 
  // Step 2 - calculate linear distance travelled by the chassis
  auto averageDeltaDistance = (deltaLeftDistance + deltaRightDistance) / 2.0;
 
  Rotation2d angle = gyroAngle + m_gyroOffset;

  // Step 3 - calculate the new robot Pose
  Pose2d newPose = m_pose.Exp(
      {averageDeltaDistance,  // Distance in the X direction
      0,                      // Distance in the Y direction (always 0 for differential drive)
      (angle - m_previousAngle).Radians()});  // Heading of the robot

  m_previousAngle = angle;

  // Return the new robot Pose
  m_pose = {newPose.Translation(), angle};
 
  return m_pose;
}
