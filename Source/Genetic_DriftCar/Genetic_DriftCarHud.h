// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Genetic_DriftCarHud.generated.h"

UCLASS(config = Game)
class AGenetic_DriftCarHud : public AHUD
{
	GENERATED_BODY()

public:
	AGenetic_DriftCarHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
