/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include <cmath>

/**
 * A change in distance along arc since the last pose update. We can use ideas
 * from differential calculus to create new Pose2ds from a Twist2d and vise
 * versa.
 *
 * A Twist can be used to represent a difference between two poses.
 */
struct Twist2d {
  /**
   * Linear "dx" component
   */
  float dx = 0;

  /**
   * Linear "dy" component
   */
  float dy = 0;

  /**
   * Angular "dtheta" component (radians)
   */
  float dtheta = 0;

  /**
   * Checks equality between this Twist2d and another object.
   *
   * @param other The other object.
   * @return Whether the two objects are equal.
   */
  bool operator==(const Twist2d& other) const {
    return std::abs(dx - other.dx) < 1E-9 &&
           std::abs(dy - other.dy) < 1E-9 &&
           std::abs(dtheta - other.dtheta) < 1E-9;
  }

  /**
   * Checks inequality between this Twist2d and another object.
   *
   * @param other The other object.
   * @return Whether the two objects are not equal.
   */
  bool operator!=(const Twist2d& other) const { return !operator==(other); }
};
