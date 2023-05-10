// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <math.h>
#include <cmath>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/MathUtil.h>
#include "DoubleArm.h"
#include <iostream>
#include <thread>

DoubleArm::DoubleArm() {
    ConfigureMotors();
    ConfigureSensors();

    // frc::SmartDashboard::PutData("arm", &plotter);

    // frc::SmartDashboard::PutNumber("Lower FeedForward", lowerFeedForward);
    // frc::SmartDashboard::PutNumber("Upper FeedForward", upperFeedForward);

}
/**
 * Will be called periodically whenever the CommandScheduler runs.
 */
void DoubleArm::Periodic() {
    auto currentState = GetCurrentState();
    frc::SmartDashboard::PutNumber("DoubleArm/CurrentLowerAngle", currentState.lowerAngle.Degrees().value());
    frc::SmartDashboard::PutNumber("DoubleArm/CurrentUpperAngle", currentState.upperAngle.Degrees().value());

    // frc::SmartDashboard::PutNumber("DoubleArm/EncoderLowerRaw", lowerEncoder.GetAbsolutePosition());
    // frc::SmartDashboard::PutNumber("DoubleArm/EncoderUpperRaw", upperEncoder.GetAbsolutePosition());

    frc::SmartDashboard::PutNumber("DoubleArm/TargetLowerAngle", targetState.lowerAngle.Degrees().value());
    frc::SmartDashboard::PutNumber("DoubleArm/TargetUpperAngle", targetState.upperAngle.Degrees().value());

    // auto targetCoord = kinematics.GetEndpointCoord(targetState);
    // plotter.SetRobotPose({ targetCoord + frc::Translation2d{4_m, 4_m}, 0_deg });
    // frc::SmartDashboard::PutNumber("DoubleArm/DesiredX", targetCoord.X().value());
    // frc::SmartDashboard::PutNumber("DoubleArm/DesiredY", targetCoord.Y().value());

    SetFalconTargetPos(targetState);

    // lowerFeedForward = frc::SmartDashboard::GetNumber("Lower FeedForward", lowerFeedForward);
    // upperFeedForward = frc::SmartDashboard::GetNumber("Upper FeedForward", upperFeedForward);

}

DoubleArmState DoubleArm::GetCurrentState() {
    DoubleArmState state;
    state.lowerAngle = GetLowerAngle();
    state.upperAngle = GetUpperAngle();
    return state;
}

void DoubleArm::SetTargetCoord(DoubleArmState targetCoord) {
    this->targetState = targetCoord;
}

void DoubleArm::SetFalconTargetPos(DoubleArmState desiredState) {
    if (desiredState.lowerAngle.Degrees() == 101_deg) {
        lowerRight.SelectProfileSlot(0, 0);
        upperRight.SelectProfileSlot(0, 0);
    } else {
        lowerRight.SelectProfileSlot(1, 0);
        upperRight.SelectProfileSlot(1, 0);
    }

    lowerRight.Set(ControlMode::MotionMagic, ConvertAngleToLowerFalconPos(desiredState.lowerAngle), DemandType_ArbitraryFeedForward, lowerFeedForward * desiredState.lowerAngle.Cos()); //0.180
    upperRight.Set(ControlMode::MotionMagic, ConvertAngleToUpperFalconPos(desiredState.upperAngle), DemandType_ArbitraryFeedForward, upperFeedForward * desiredState.upperAngle.Cos()); //0.060
}

void DoubleArm::ConfigureMotors() {
    TalonFXConfiguration baseConfig;
    baseConfig.voltageCompSaturation = 12.0;
    baseConfig.supplyCurrLimit = SupplyCurrentLimitConfiguration(true, 25, 30, 0);
    baseConfig.neutralDeadband = 0.001;

    /* Lower Motors */
    lowerRight.ConfigAllSettings(baseConfig);
    lowerRight.EnableVoltageCompensation(true);
    lowerRight.SetNeutralMode(NeutralMode::Brake);

    lowerRight2.ConfigAllSettings(baseConfig);
    lowerRight2.EnableVoltageCompensation(true);
    lowerRight2.SetNeutralMode(NeutralMode::Brake);
    lowerRight2.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_1_General, 255);
    lowerRight2.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_2_Feedback0, 255);


    lowerLeft.ConfigAllSettings(baseConfig);
    lowerLeft.EnableVoltageCompensation(true);
    lowerLeft.SetNeutralMode(NeutralMode::Brake);
    lowerLeft.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_1_General, 255);
    lowerLeft.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_2_Feedback0, 255);

    lowerLeft2.ConfigAllSettings(baseConfig);
    lowerLeft2.EnableVoltageCompensation(true);
    lowerLeft2.SetNeutralMode(NeutralMode::Brake);
    lowerLeft2.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_1_General, 255);
    lowerLeft2.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_2_Feedback0, 255);

    lowerRight2.Follow(lowerRight);
    lowerLeft2.Follow(lowerRight);
    lowerLeft.Follow(lowerRight);

    lowerRight2.SetInverted(TalonFXInvertType::FollowMaster);
    lowerLeft.SetInverted(TalonFXInvertType::OpposeMaster);
    lowerLeft2.SetInverted(TalonFXInvertType::OpposeMaster);

    lowerRight.SelectProfileSlot(0, 0);
    lowerRight.Config_kP(0, lowerKP); // 0.085
    lowerRight.Config_kI(0, 0);
    lowerRight.Config_kD(0, 0); // 10

    lowerRight.Config_kP(1, lowerKP);
    lowerRight.Config_kI(1, 0);
    lowerRight.Config_kD(1, 0);

    lowerRight.ConfigMotionSCurveStrength(6);
    lowerRight.ConfigMotionCruiseVelocity(21700); // 21700??
    lowerRight.ConfigMotionAcceleration(20000);

    /* Upper Motors */
    upperRight.ConfigAllSettings(baseConfig);
    upperRight.EnableVoltageCompensation(true);
    upperRight.SetNeutralMode(NeutralMode::Brake);

    upperLeft.ConfigAllSettings(baseConfig);
    upperLeft.EnableVoltageCompensation(true);
    upperLeft.SetNeutralMode(NeutralMode::Brake);

    upperRight.SetInverted(true);
    upperLeft.SetInverted(InvertType::FollowMaster);
    upperLeft.Follow(upperRight);
    upperLeft.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_1_General, 255);
    upperLeft.SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrameEnhanced::Status_2_Feedback0, 255);

    upperRight.SelectProfileSlot(0, 0);
    upperRight.Config_kP(0, upperKP); // 0.065
    upperRight.Config_kI(0, 0);
    upperRight.Config_kD(0, 0); // 10

    upperRight.Config_kP(1, upperKP); // 0.065
    upperRight.Config_kI(1, 0);
    upperRight.Config_kD(1, 0); // 10

    upperRight.ConfigMotionSCurveStrength(6);
    upperRight.ConfigMotionCruiseVelocity(21700);
    upperRight.ConfigMotionAcceleration(20000);
}

void DoubleArm::ConfigureSensors() {
    lowerRight.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor);
    upperRight.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    lowerRight.SetSelectedSensorPosition(ConvertAngleToLowerFalconPos(GetLowerAngle()));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    upperRight.SetSelectedSensorPosition(ConvertAngleToUpperFalconPos(GetUpperAngle()));
}

frc::Rotation2d DoubleArm::GetLowerAngle() {
    return units::degree_t((lowerEncoder.GetAbsolutePosition() - 0.3660) * 360.0);
}

frc::Rotation2d DoubleArm::GetUpperAngle() {
    double rawVal = (-upperEncoder.GetAbsolutePosition() + 0.3074) * 360.0;
    rawVal = frc::InputModulus(rawVal, -180.0, 180.0);
    frc::Rotation2d angleToLowerArm = frc::Rotation2d(units::degree_t(rawVal));

    // frc::SmartDashboard::PutNumber("DoubleArm/AngleUpperToLower", angleToLowerArm.Degrees().value());
    return angleToLowerArm + GetLowerAngle();
}

double DoubleArm::ConvertAngleToLowerFalconPos(frc::Rotation2d angle) {
    return angle.Degrees().value() / 360 * CODES_PER_LOWER_ROTATION;
}
double DoubleArm::ConvertAngleToUpperFalconPos(frc::Rotation2d angle) {
    frc::Rotation2d targetAngle = angle - GetLowerAngle();
    return targetAngle.Degrees().value() / 360 * CODES_PER_UPPER_ROTATION;
}