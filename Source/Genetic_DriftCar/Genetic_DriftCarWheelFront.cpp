// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Genetic_DriftCarWheelFront.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"

UGenetic_DriftCarWheelFront::UGenetic_DriftCarWheelFront()
{
	ShapeRadius = 18.0f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 60.f;

	// Setup suspension forces
	SuspensionForceOffset = -4.0f;
	SuspensionMaxRaise = 4.0f;
	SuspensionMaxDrop = 4.0f;
	SuspensionNaturalFrequency = 15.0f;
	SuspensionDampingRatio = 1.05f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/VehicleAdv/Vehicle/WheelData/Vehicle_FrontTireConfig.Vehicle_FrontTireConfig"));
	TireConfig = TireData.Object;
}
