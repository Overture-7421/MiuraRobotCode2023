// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <Subsystems/SwerveModule/SwerveModule.h>

#include <frc/Joystick.h>
#include <frc/geometry/Translation2d.h>
#include <frc/geometry/Pose2d.h>
#include <frc/kinematics/SwerveDriveKinematics.h>
#include <frc/kinematics/ChassisSpeeds.h>
#include <frc/kinematics/SwerveDriveOdometry.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/Field2d.h>
#include <frc/estimator/SwerveDrivePoseEstimator.h>
#include <frc2/command/SubsystemBase.h>
#include <numbers>
#include <AHRS.h>
//#include <frc/filter/SlewRateLimiter.h>

class SwerveChassis : public frc2::SubsystemBase {
 public:
  SwerveChassis();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

 private:
  SwerveModule backRightModule{ 1, 2, 9, -143.70507812500001 };
  SwerveModule backLeftModule{ 3, 4, 10, -70 };
  SwerveModule frontLeftModule{ 5, 6, 11, -147.5 };
  SwerveModule frontRightModule{ 7, 8, 12, -160 };

  double wheelVoltage;
  double targetAngle;

  double linearX;
  double linearY;
  double angular;

  std::array<frc::Translation2d, 4> modulePos{
      frc::Translation2d(10.36_in, 10.36_in),   // front left
      frc::Translation2d(10.36_in, -10.36_in),  // front right
      frc::Translation2d(-10.36_in, -10.36_in), // back right
      frc::Translation2d(-10.36_in, 10.36_in)   // back left
  };

  AHRS navx{ frc::SPI::Port::kMXP };
  frc::SwerveDriveKinematics<4> kinematics{ modulePos };

  frc::SwerveDrivePoseEstimator<4> odometry{ kinematics, frc::Rotation2d{}, modulePos, frc::Pose2d{}};

  frc::Field2d field2d;
};
