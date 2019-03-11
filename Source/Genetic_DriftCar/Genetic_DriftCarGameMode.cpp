// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Genetic_DriftCarGameMode.h"
#include "Genetic_DriftCarPawn.h"
#include "Genetic_DriftCarHud.h"

AGenetic_DriftCarGameMode::AGenetic_DriftCarGameMode()
{
	DefaultPawnClass = AGenetic_DriftCarPawn::StaticClass();
	HUDClass = AGenetic_DriftCarHud::StaticClass();
}
