// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc/XboxController.h>
#include <frc2/command/button/Trigger.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc/smartdashboard/SendableChooser.h>

#include "Subsystems/SwerveChassis/SwerveChassis.h"
#include "Subsystems/VisionManager/VisionManager.h"
#include "Subsystems/DoubleArm/DoubleArm.h"
#include "Subsystems/Intake/Intake.h"

#include "Commands/Teleop/Drive/Drive.h"
#include "Commands/Common/AlignRobotToTarget/AlignRobotToTarget.h"
#include "Commands/Common/IntakeControl/IntakeControl.h"
#include "Commands/Common/AutoBalance/AutoBalance.h"
#include "Commands/Common/SetWrist/SetWrist.h"
#include "Commands/Common/SetCone/SetCone.h"
#include "Commands/Common/SetintakeSpeed/SetIntakeSpeed.h"
#include "Commands/Common/SetArmCoordinate/SetArmCoordinate.h"

/* Autonomous */
#include "Commands/Autonomous/LoadingDouble.h"
#include "Commands/Autonomous/LoadingBalance.h"
#include "Commands/Autonomous/BarrierDouble.h"
#include "Commands/Autonomous/BarrierBalance.h"
#include "Commands/Autonomous/CenterBalance.h"
#include "Commands/Autonomous/DropCone.h"
#include "Commands/Autonomous/DropConeAndMove.h"

#include <Subsystems/DoubleArm/ArmConstants.h>

using namespace ArmConstants;

class RobotContainer {
public:
    RobotContainer();
    frc2::Command* GetAutonomousCommand();
    void setDriveCommand();
    // void setVisionManager();
    SwerveChassis swerveChassis;

private:
    void ConfigureBindings();

    // Chassis driver controller and buttons
    frc::XboxController controller{ 0 };
    frc2::Trigger resetNavx{ [this] {return controller.GetBackButton();} };
    frc2::Trigger autoBalance{ [this] {return controller.GetStartButton();} };

    // Mechanism Controller
    frc::XboxController mechanisms{ 1 };

    //Pneumatics
    frc2::Trigger conePiston{ [this] {return mechanisms.GetRightBumper();} };
    frc2::Trigger wristPiston{ [this] {return mechanisms.GetLeftBumper();} };

    //Positions
    frc2::Trigger upperPosition{ [this] {return mechanisms.GetYButton();} }; //Upper
    frc2::Trigger middlePosition{ [this] {return mechanisms.GetXButton();} }; //Middle
    frc2::Trigger groundPickUp{ [this] {return mechanisms.GetAButton();} }; //Ground
    frc2::Trigger portalPosition{ [this] {return mechanisms.GetBButton();} }; //Portal
    frc2::Trigger lowerPosition{ [this] {return mechanisms.GetPOV(0.75);} }; //Closed

    // Subsystems
    Intake intake;
    DoubleArm doubleArm;


    frc::SendableChooser<frc2::Command*> pathChooser;
    frc2::CommandPtr loadingDouble = LoadingDouble(&swerveChassis, &doubleArm, &intake);
    frc2::CommandPtr loadingBalance = LoadingBalance(&swerveChassis, &doubleArm, &intake);
    frc2::CommandPtr barrierBalance = BarrierBalance(&swerveChassis, &doubleArm, &intake);
    frc2::CommandPtr barrierDouble = BarrierDouble(&swerveChassis, &doubleArm, &intake);
    frc2::CommandPtr centerBalance = CenterBalance(&swerveChassis, &doubleArm, &intake);
    frc2::CommandPtr dropCone = DropCone(&swerveChassis, &doubleArm, &intake);
    frc2::CommandPtr dropConeAndMove = DropConeAndMove(&swerveChassis, &doubleArm, &intake);
};